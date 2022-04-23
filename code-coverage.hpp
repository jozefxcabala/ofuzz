#include "vector"
#include "cstdint"
#include "string"

#ifndef CODE_COVERAGE_H
#define CODE_COVERAGE_H

class CodeCoverage 
{
    int coverage_;
    std::vector<uint64_t> data_;

    public:
        CodeCoverage();

        void setCoverage(int coverage);
        void setData(std::vector<uint64_t> data);
        
        std::vector<uint64_t> data();    
        int coverage();

        void start();
        void printData();

    private:
        bool checkForUniqueValue(std::vector<uint64_t> data, uint64_t value);
        void createMyFifo();
        void redirectSTDOut();
        void runInstrumentedBinaryFile();
        void closeMyFifo();
        void calculateCoverage();    
        std::vector<uint64_t> saveTheReachedBlocks(); 
        int getNumberOfBlocks();   
};

#endif // CODE_COVERAGE_H