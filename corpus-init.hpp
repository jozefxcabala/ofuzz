#include <string>
#include <queue>
#include <vector>
#include "sample-processing.hpp"
#include "sample.hpp"

#ifndef CORPUS_INIT_H
#define CORPUS_INIT_H

class CorpusInit 
{
    std::string dirForInputSamples_;
    int sizeOfCorpus_;
    SampleProcessing sampleProcessing_;

    public:
        CorpusInit();
        CorpusInit(std::string dirForInputSamples, SampleProcessing sampleProcessing, int sizeOfCorpus);

        std::queue<Sample> start();

        void setDirForInputSamples(std::string dirForInputSamples);
        void setSampleProcessing(SampleProcessing sampleProcessing);
        void setSizeOfCorpus(int sizeOfCorpus);

        std::string dirForInputSamples();
        SampleProcessing sampleProcessing();
        int sizeOfCorpus();

    private:
        std::queue<Sample> createNew();
        std::vector<std::string> getListOfFiles();
        void copyInputFiles(int count, std::string data, std::string fileName);
};

#endif // CORPUS_INIT_H