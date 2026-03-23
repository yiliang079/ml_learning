#include <iostream>
#include <string>
#include <cstdint>

class RegisterBlock {
private:
    // 1. 私有属性 (类似内部 reg)
    uint64_t base_addr;
    uint32_t size;
    bool is_locked;

public:
    // 2. 构造函数 (Constructor)：对象创建时自动执行，类似硬件 Reset
    RegisterBlock(uint64_t addr, uint32_t s) 
        : base_addr(addr), size(s), is_locked(false) {
        std::cout << "[LOG] RegisterBlock @0x" << std::hex << base_addr 
                  << " Initialized (Size: " << std::dec << size << " bytes)" << std::endl;
    }

    // 3. 析构函数 (Destructor)：对象销毁时执行，用于清理资源
    ~RegisterBlock() {
        std::cout << "[LOG] RegisterBlock @0x" << std::hex << base_addr << " Released." << std::endl;
    }

    // 4. 公有成员函数 (类似 Module 的接口)
    void write(uint64_t addr, uint32_t data) {
        if (is_locked) {
            std::cout << "[ERROR] Write Denied! Register is LOCKED." << std::endl;
            return;
        }
        std::cout << "[WRITE] Writing " << std::dec << data << " to 0x" << std::hex << addr << std::endl;
    }

    void set_lock(bool lock_state) {
        is_locked = lock_state;
    }
};

int main() {
    std::cout << "--- Starting Simulation ---" << std::endl;

    // 实例化类 (在内存中创建这个“硬件模块”)
    RegisterBlock gpu_reg(0x40001000, 1024);

    gpu_reg.write(0x40001004, 555); // 正常写入
    
    gpu_reg.set_lock(true);        // 锁定寄存器
    gpu_reg.write(0x40001008, 999); // 写入将被拒绝

    std::cout << "--- End of Simulation ---" << std::endl;
    // 函数结束时，gpu_reg 超出作用域，会自动调用析构函数
    return 0;
}