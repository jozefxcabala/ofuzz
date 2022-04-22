#include "binary-file-instrumentation.hpp"
#include "code-coverage.hpp"
#include "iostream"

int main()
{
    BinaryFileInstrumentation binaryFileInstrumentation;
    CodeCoverage codeCoverage;

    binaryFileInstrumentation.start();
    codeCoverage.start();
    codeCoverage.printData();
 
    return 0;
}