#include "vector"
#include "cstdint"
#include "string"

#ifndef BINARY_FILE_INSTRUMENTATION_H
#define BINARY_FILE_INSTRUMENTATION_H

class BinaryFileInstrumentation 
{
    std::string myFifo_;
    std::vector<uint64_t> data_;
    int saveOut_;

    public:
        BinaryFileInstrumentation();
        bool start();

        void setData(std::vector<uint64_t> data);
        void setSaveOut(int saveOut);

        std::vector<uint64_t> data();
        int saveOut();
        void printData();

    private:
        bool createMyFifo();
        bool runE9PatchTool();
        bool redirectSTDOut();
        bool runInstrumentedBinaryFile();
        std::vector<uint64_t> saveTheReachedBlocks();
        bool closeMyFifo();
        
};

#endif // BINARY_FILE_INSTRUMENTATION_H