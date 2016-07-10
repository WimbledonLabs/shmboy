#include "main.h"
#include <iostream>

#include "cpu.h"

int main(int argc, char **argv) {
    Cpu cpu;
    std::cout << "Hello World! " << cpu.pc << std::endl;
    return 0;
}
