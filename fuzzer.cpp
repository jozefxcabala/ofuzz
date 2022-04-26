#include "fuzzer.hpp"
#include <iostream>

Fuzzer::Fuzzer()
{

}

Fuzzer::Fuzzer(SampleProcessing sampleProcessingA, Mutation mutationA, CrashesProcessing crashesProcessingA, std::queue<Sample> corpusA)
{   
    setMutation(mutationA);
    setSampleProcessing(sampleProcessingA);
    setCrashesProcessing(crashesProcessingA);
    setCorpus(corpusA);
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

void Fuzzer::start()
{
    std::cout << "uspesne som spustil fuzzer!" << std::endl;
}