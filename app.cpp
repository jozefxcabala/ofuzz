#include "binary-file-instrumentation.hpp"
#include "iostream"

int main()
{
    BinaryFileInstrumentation binaryFileInstrumentation;
    std::cout << binaryFileInstrumentation.start() << std::endl;

    
    return 0;
}