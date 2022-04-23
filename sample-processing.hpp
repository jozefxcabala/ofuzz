#include <string>

#ifndef SAMPLE_PROCESSING_H
#define SAMPLE_PROCESSING_H

class SampleProcessing 
{
    public:
        SampleProcessing();

        std::string getBytes(std::string fileName);
        void createNew(std::string mutated, std::string fileName);
};

#endif // SAMPLE_PROCESSING_H