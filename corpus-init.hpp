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
    int argc_;

    public:
        CorpusInit();
        CorpusInit(char** argv, int sizeOfCorpus, int argc);

        std::queue<Sample> start();

        void setArgv(char** argv);
        void setSizeOfCorpus(int sizeOfCorpus);
        void setArgc(int argc);

        char** argv();
        int sizeOfCorpus();
        int argc();

    private:
        std::queue<Sample> createNew();
        std::vector<std::string> getListOfFiles();
        void copyInputFiles(int count, SampleProcessing sampleProcessing, std::string fileName);
};

#endif // CORPUS_INIT_H