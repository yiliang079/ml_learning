import time
from transformers import pipeline
import torch

# 明确指定设备：如果有显卡用 "cuda"，没有就用 "cpu"
device = 0 if torch.cuda.is_available() else -1

print("正在加载模型...")
start_load = time.time()
# 增加 torch_dtype=torch.float16 可以大幅提速（如果显卡支持）
generator = pipeline("text-generation", model="Qwen/Qwen2.5-0.5B-Instruct", device=device)
print(f"模型加载耗时: {time.time() - start_load:.2f} 秒")

prompt = "What is GPU"

print("正在生成回答...")
start_gen = time.time()
# 加上 clean_up_tokenization_spaces 让输出更干净
result = generator(prompt, max_new_tokens=200, do_sample=True, temperature=0.7)
print(f"推理计算耗时: {time.time() - start_gen:.2f} 秒")

print("\n结果如下：")
print(result[0]["generated_text"])