#include <string>

#ifndef SAMPLE_PROCESSING_H
#define SAMPLE_PROCESSING_H

class SampleProcessing 
{
    int id_;

    public:
        SampleProcessing();
        SampleProcessing(int id);

        std::string getBytes(std::string fileName, std::string dirName);
        int id();

        void createNew(std::string data, std::string fileName, std::string dirName);
        void setId(int id);

        
};

#endif // SAMPLE_PROCESSING_H