#include "sample.hpp"

Sample::Sample()
{

}

Sample::Sample(SampleProcessing sampleProcessingA, CodeCoverage codeCoverageA, std::string fileNameA, CrashesProcessing crashesProcessingA, Mutation mutationA)
{
    setSampleProcessing(sampleProcessingA);
    setCodeCoverage(codeCoverageA);
    setFileName(fileNameA);
    setCrashesProcessing(crashesProcessingA);
    setMutation(mutationA);
}

std::string Sample::fileName()
{
    return fileName_;
}

CodeCoverage Sample::codeCoverage()
{
    return codeCoverage_;
}

SampleProcessing Sample::sampleProcessing()
{
    return sampleProcessing_;
}

CrashesProcessing Sample::crashesProcessing()
{
    return crashesProcessing_;
}

Mutation Sample::mutation()
{
    return mutation_;
}

void Sample::setFileName(std::string fileName)
{
    fileName_ = fileName;
}

void Sample::setCodeCoverage(CodeCoverage codeCoverage)
{
    codeCoverage_ = codeCoverage;
}

void Sample::setSampleProcessing(SampleProcessing sampleProcessing)
{   
    sampleProcessing_ = sampleProcessing;
}

void Sample::setCrashesProcessing(CrashesProcessing crashesProcessing)
{
    crashesProcessing_ = crashesProcessing;
}

void Sample::setMutation(Mutation mutation)
{
    mutation_ = mutation;
}