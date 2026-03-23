#include <iostream>
#include <vector>   // 必须包含：动态数组容器
#include <string>
#include <cstdint>
#include <algorithm> // 包含排序等算法

int main() {
    // 1. std::vector 与 初始化列表
    // 想象这是你要处理的一组 GPU 寄存器地址
    std::vector<uint64_t> addresses = {0x7000, 0x8000, 0x1000, 0x3000};

    // 2. 使用 auto 和 range-based for 循环
    // 编译器会自动推导出 addr 是 uint64_t 类型
    std::cout << "Original Addresses:" << std::endl;
    for (auto addr : addresses) { 
        std::printf("  Addr: 0x%llX\n", addr);
    }

    // 3. 动态操作：插入和删除
    addresses.push_back(0xA000); // 在末尾添加一个地址
    addresses.erase(addresses.begin()); // 删除第一个元素 (0x7000)

    // 4. 排序 (std::sort)
    // 这是标准库提供的强大功能，不需要你自己写冒泡排序
    std::sort(addresses.begin(), addresses.end());

    // 5. 使用引用 (&) 避免拷贝
    // 在处理大数据时，用 const auto& 可以极大提升性能（类似 Verilog 里的引用连接）
    std::cout << "\nSorted Addresses (Optimized Access):" << std::endl;
    for (const auto& addr : addresses) {
        std::printf("  Addr: 0x%llX\n", addr);
    }

    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}