#include <string>

#ifndef SAMPLE_PROCESSING_H
#define SAMPLE_PROCESSING_H

class SampleProcessing 
{

    public:
        SampleProcessing();

        std::string getBytes(std::string fileName, std::string dirName);
        void createNew(std::string data, std::string fileName, std::string dirName);
};

#endif // SAMPLE_PROCESSING_H