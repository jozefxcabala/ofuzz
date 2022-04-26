#include <string>
#include <queue>
#include "sample.hpp"
#include "sample-processing.hpp"
#include "mutation.hpp"
#include "crashes-processing.hpp"

#ifndef FUZZER_H
#define FUZZER_H

class Fuzzer 
{
    SampleProcessing sampleProcessing_;
    Mutation mutation_;
    CrashesProcessing crashesProcessing_;

    std::queue<Sample> corups_;

    public:
        Fuzzer();
        Fuzzer(SampleProcessing sampleProcessing, Mutation mutation, CrashesProcessing crashesProcessing, std::queue<Sample> corpus);

        SampleProcessing sampleProcessing();
        Mutation mutation();
        CrashesProcessing crashesProcessing();
        std::queue<Sample> corpus();

        void setSampleProcessing(SampleProcessing sampleProcessing);
        void setMutation(Mutation mutation);
        void setCrashesProcessing(CrashesProcessing crashesProcessing);
        void setCorpus(std::queue<Sample> corpus);

        void start();

};

#endif // FUZZER_H