#include <iostream>
#include <thread>
#include <chrono> // 必须加这个头文件来使用时间

void worker(std::string name) {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread [" << name << "] processing data " << i << std::endl;
        // 模拟复杂计算，让出 CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
    }
}

int main() {
    std::cout << "Starting multi-threaded verification..." << std::endl;

    std::thread t1(worker, "Sub-Core-0");

    for (int i = 0; i < 5; ++i) {
        std::cout << "Main-Core is handling interrupts " << i << std::endl;
        // 同样模拟延迟
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
    }

    t1.join();
    std::cout << "All tasks finished. Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}