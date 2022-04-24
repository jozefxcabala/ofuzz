#include <string>

#ifndef SAMPLE_PROCESSING_H
#define SAMPLE_PROCESSING_H

class SampleProcessing 
{
    std::string difForMutatedFiles_;

    public:
        SampleProcessing();
        SampleProcessing(std::string dirForMutatedFiles);

        std::string getBytes(std::string fileName);
        void createNew(std::string mutated, std::string fileName);

        void setDirForMutatedFiles(std::string dirForMutatedFiles);
        std::string dirForMutatedFiles();
};

#endif // SAMPLE_PROCESSING_H