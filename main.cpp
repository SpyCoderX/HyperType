#include <iostream>
#include <string>
#include "src/head/lang/Processor.h"
#include <windows.h>
#include <bitset>

int main() {
    ProcessorTests::runTests();
    std::cout << "Press ENTER to continue..." << std::endl;
    std::cin.get();
    
    return 0;
} 