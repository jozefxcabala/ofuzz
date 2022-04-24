#include "fuzzer.hpp"

Fuzzer::Fuzzer()
{

}

Fuzzer::Fuzzer(BinaryFileInstrumentation binaryFileInstrumentation, SampleProcessing sampleProcessing, Mutation mutation, CrashesProcessing crashesProcessing, std::queue<Sample> corpus)
{   
    setBinaryFileInstrumentation(binaryFileInstrumentation);
    setMutation(mutation);
    setSampleProcessing(sampleProcessing);
    setCrashesProcessing(crashesProcessing);
    setCorpus(corpus);
}

void Fuzzer::setBinaryFileInstrumentation(BinaryFileInstrumentation binaryFileInstrumentation)
{
    binaryFileInstrumentation_ = binaryFileInstrumentation;
}

void Fuzzer::setCorpus(std::queue<Sample> corpus)
{
    corups_ = corpus;
}

void Fuzzer::setMutation(Mutation mutation)
{
    mutation_ = mutation;
}

void Fuzzer::setSampleProcessing(SampleProcessing sampleProcessing)
{
    sampleProcessing_ = sampleProcessing;
}

void Fuzzer::setCrashesProcessing(CrashesProcessing crashesProcessing)
{
    crashesProcessing_ = crashesProcessing;
}

BinaryFileInstrumentation Fuzzer::binaryFileInstrumentation()
{
    return binaryFileInstrumentation_;
}

std::queue<Sample> Fuzzer::corpus()
{
    return corups_;
}

Mutation Fuzzer::mutation()
{
    return mutation_;
}

SampleProcessing Fuzzer::sampleProcessing()
{
    return sampleProcessing_;
}

CrashesProcessing Fuzzer::crashesProcessing()
{
    return crashesProcessing_;
}