#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

int main() {
    std::vector<uint64_t> addresses = {0x1001, 0x2000, 0x3005, 0x4002};
    uint64_t offset = 0x100; // 外部变量

    std::cout << "--- Processing with Lambda ---" << std::endl;

    // 使用 std::for_each 配合 Lambda
    // [&] 表示“按引用捕获”所有外部变量，这样 Lambda 就能看到并修改 offset 或 addresses
    std::for_each(addresses.begin(), addresses.end(), [&](uint64_t& addr) {
        if (addr % 2 != 0) { // 如果是奇数地址
            std::cout << "Modifying Odd Addr: 0x" << std::hex << addr;
            addr += offset;  // 使用了外部变量 offset
            std::cout << " -> 0x" << addr << std::endl;
        }
    });

    std::cout << "Final addresses:" << std::endl;
    for (const auto& a : addresses) {
        std::cout << "  0x" << std::hex << a << std::endl;
    }

    return 0;
}