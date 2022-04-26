#include <string>
#include <queue>
#include <vector>
#include "sample-processing.hpp"
#include "sample.hpp"

#ifndef CORPUS_INIT_H
#define CORPUS_INIT_H

class CorpusInit 
{
    char** argv_;
    int sizeOfCorpus_;

    public:
        CorpusInit();
        CorpusInit(char** argv, int sizeOfCorpus);

        std::queue<Sample> start();

        void setArgv(char** argv);
        void setSizeOfCorpus(int sizeOfCorpus);

        char** argv();
        int sizeOfCorpus();

    private:
        std::queue<Sample> createNew();
        std::vector<std::string> getListOfFiles();
        void copyInputFiles(int count, SampleProcessing sampleProcessing, std::string fileName);
};

#endif // CORPUS_INIT_H