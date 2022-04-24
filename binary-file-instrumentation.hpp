#include "vector"
#include "cstdint"
#include "string"

#ifndef BINARY_FILE_INSTRUMENTATION_H
#define BINARY_FILE_INSTRUMENTATION_H

class BinaryFileInstrumentation
{
    std::string targetApplication_;

    public:
        BinaryFileInstrumentation();
        BinaryFileInstrumentation(std::string targetApplication);
        void start();

        void setTargetApplication(std::string targetApplication);
        std::string targetApplication();

    private:
        void runE9PatchTool();
};

#endif // BINARY_FILE_INSTRUMENTATION_H