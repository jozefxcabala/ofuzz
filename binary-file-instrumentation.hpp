#include "vector"
#include "cstdint"
#include "string"

#ifndef BINARY_FILE_INSTRUMENTATION_H
#define BINARY_FILE_INSTRUMENTATION_H

class BinaryFileInstrumentation 
{
    public:
        BinaryFileInstrumentation();
        void start();

    private:
        void runE9PatchTool();
};

#endif // BINARY_FILE_INSTRUMENTATION_H