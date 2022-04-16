#include "binary-file-instrumentation.hpp"
#include "iostream"

int main()
{
    BinaryFileInstrumentation binaryFileInstrumentation;
    binaryFileInstrumentation.start();
    binaryFileInstrumentation.printData();
 
    return 0;
}