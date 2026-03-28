import GPUtil
import time

def monitor_gpu():
    print(f"{'ID':<3} {'Name':<15} {'VRAM %':<10} {'VRAM Free':<12} {'Temp':<5} {'Load':<5}")
    try:
        while True:
            gpus = GPUtil.getGPUs()
            for gpu in gpus:
                # 打印核心数据
                print(f"{gpu.id:<3} {gpu.name:<15} {gpu.memoryUtil*100:2.1f}%     {gpu.memoryFree:4.0f}MB      {gpu.temperature:2.0f}C   {gpu.load*100:2.1f}%", end='\r')
            time.sleep(0.5) # 每0.5秒刷新一次
    except KeyboardInterrupt:
        print("\n监测停止")

if __name__ == "__main__":
    monitor_gpu()


    