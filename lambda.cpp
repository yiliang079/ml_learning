#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

struct Packet {
    int id;
    int size;
};


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

//--------------------------------------------------------------------------
//test3
    std::cout << "---------test3--------- "  << std::endl;

    std::vector<Packet> stream = {{1, 64}, {2, 1518}, {3, 256}, {4, 2000}};
    int max_mtu = 1500;
    int dropped_count = 0;

    // 练习：使用 Lambda 过滤并计数
    std::for_each(stream.begin(), stream.end(), [max_mtu, &dropped_count](const Packet& p) {
        if (p.size > max_mtu) {
            std::cout << "Dropping Packet ID: " << p.id << " (Size too large)\n";
            dropped_count++; // 引用捕获，直接修改外部变量
        }
    });

    std::cout << "Total dropped: " << dropped_count << std::endl;
    return 0;
}