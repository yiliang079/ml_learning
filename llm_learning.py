import time
import fitz  # PyMuPDF
from transformers import AutoModelForCausalLM, AutoTokenizer, BitsAndBytesConfig
import torch
from modelscope import snapshot_download
import os

def extract_text_from_pdf(pdf_path):
    doc = fitz.open(pdf_path)
    text = ""
    for page in doc:
        text += page.get_text()
    return text


os.environ['HF_HOME'] = "E:/huggingface_cache"
os.environ['HUGGINGFACE_HUB_CACHE'] = "E:/huggingface_cache"

from transformers import pipeline
device = 0 if torch.cuda.is_available() else -1
MODEL_ID = "Qwen/Qwen2.5-7B-Instruct"
custom_path = "E:/huggingface_cache"
CUSTOM_CACHE_DIR = "E:/huggingface_cache"
local_dir = snapshot_download(MODEL_ID, cache_dir=CUSTOM_CACHE_DIR)
TARGET_STEP = 20    # <--- 报错就是因为缺了这一行
WORDS_PER_STEP = 50 # <--- 这一行通常也在一起


print("正在加载模型...")
start_load = time.time()
# 增加 torch_dtype=torch.float16 可以大幅提速（如果显卡支持）

generator = pipeline(
    "text-generation", 
    model=MODEL_ID, 
    device=device, # 2080ti 对应 device 0
    model_kwargs={"cache_dir": custom_path} 
)

quantization_config = BitsAndBytesConfig(
    load_in_4bit=True,
    bnb_4bit_compute_dtype=torch.float16,
    bnb_4bit_quant_type="nf4",
    bnb_4bit_use_double_quant=True
)

print(f"\n正在以 4-bit 量化模式加载模型到 2080 Ti...")
tokenizer = AutoTokenizer.from_pretrained(local_dir)
model = AutoModelForCausalLM.from_pretrained(
    local_dir,
    quantization_config=quantization_config,
    device_map="cuda:0" # 强制在第一张卡
)
print("模型加载完成。")

# ==========================================
# 4. 显存吞噬实验：Prefill & Decode
# ==========================================

# 构造一个需要初步思考的长 Prompt（触发 Prefill）
prompt = "介绍一下Rdma技术的原理和应用场景。" * 10 # 通过重复来增加 Token 数量
inputs = tokenizer(prompt, return_tensors="pt").to("cuda")
prompt_tokens = inputs.input_ids.shape[1]

print(f"\n[实验开始]")
print(f"Prompt Token 数: {prompt_tokens} (即将触发 Prefill)")
time.sleep(2) # 给你留出看 nvitop 的时间

print("\n--- 正在进行 Prefill (生成第1个词) ---")
# 第一次生成包含了 Prefill，显存里会生成第一批 KV Cache
with torch.no_grad():
    outputs = model.generate(
        **inputs, 
        max_new_tokens=1, 
        use_cache=True, 
        pad_token_id=tokenizer.eos_token_id
    )
    generated_ids = outputs

print("\n--- 正在进行 Decode (长文本生成) ---")
print("请在 nvitop 窗口观察 VRAM Used 数字的连续上升。")

# 逐步生成，每次蹦 50 个词
for step in range(TARGET_STEP):
    with torch.no_grad():
        outputs = model.generate(
            generated_ids,
            max_new_tokens=WORDS_PER_STEP,
            do_sample=True,
            use_cache=True, # 必须开启 KV Cache
            temperature=0.7,
            top_p=0.9,
            pad_token_id=tokenizer.eos_token_id
        )
        generated_ids = outputs
        
    current_tokens = generated_ids.shape[1]
    # ASIC 计算：Qwen2.5-7B 每 Token 的 KV 大约 28KB
    est_kv_mb = (current_tokens - prompt_tokens) * 28 / 1024
    
    print(f"Step {step+1}/{TARGET_STEP} | 总 Token 数: {current_tokens} | 累计 Decode: {current_tokens - prompt_tokens} | 预计 KV Cache 占用: ~{est_kv_mb:.1f} MB")

print("\n[实验完成]")
print("模型生成的文本片段：\n")
# 打印最后 200 个词看看效果
print(tokenizer.decode(generated_ids[0][-1000:], skip_special_tokens=True))
