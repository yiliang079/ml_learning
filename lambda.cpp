#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

int main() {


//--------------------------------------------------------------------------
//test1

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
//--------------------------------------------------------------------------
//test2
std::vector<int> latencies = {300, 700, 450, 1200, 100};
    int threshold = 500;
    int count = 0;

    // 使用 Lambda 配合 std::count_if
    // [&] 表示引用捕获 threshold 和 count
    count = std::count_if(latencies.begin(), latencies.end(), [&](int l) {
        return l > threshold; 
    });

    std::cout << "High latency packets: " << count << std::endl;

    // 顺便排个序
    std::sort(latencies.begin(), latencies.end(), [](int a, int b) {
        return a < b; // 从小到大
    });


    return 0;
}