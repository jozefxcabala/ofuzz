#include <string>
#include <queue>
#include "binary-file-instrumentation.hpp"
#include "sample.hpp"
#include "sample-processing.hpp"
#include "mutation.hpp"
#include "crashes-processing.hpp"

#ifndef FUZZER_H
#define FUZZER_H

class Fuzzer 
{
    BinaryFileInstrumentation binaryFileInstrumentation_;
    SampleProcessing sampleProcessing_;
    Mutation mutation_;
    CrashesProcessing crashesProcessing_;

    std::queue<Sample> corups_;

    public:
        Fuzzer();
        Fuzzer(BinaryFileInstrumentation binaryFileInstrumentation, SampleProcessing sampleProcessing, Mutation mutation, CrashesProcessing crashesProcessing, std::queue<Sample> corpus);

        BinaryFileInstrumentation binaryFileInstrumentation();
        SampleProcessing sampleProcessing();
        Mutation mutation();
        CrashesProcessing crashesProcessing();
        std::queue<Sample> corpus();

        void setBinaryFileInstrumentation(BinaryFileInstrumentation binaryFileInstrumentation);
        void setSampleProcessing(SampleProcessing sampleProcessing);
        void setMutation(Mutation mutation);
        void setCrashesProcessing(CrashesProcessing crashesProcessing);
        void setCorpus(std::queue<Sample> corpus);

        void start();

};

#endif // FUZZER_H