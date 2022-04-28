#include "corpus-init.hpp"
#include "sample-processing.hpp"
#include "mutation.hpp"
#include "crashes-processing.hpp"

#include <filesystem>
#include <iostream>

CorpusInit::CorpusInit()
{

}

CorpusInit::CorpusInit(char** argv, int sizeOfCorpus, int argc)
{
    setArgv(argv);
    setSizeOfCorpus(sizeOfCorpus);
    setArgc(argc);
}

std::queue<Sample> CorpusInit::start()
{
    return createNew();
}

void CorpusInit::setArgv(char** argv)
{
    argv_ = argv;
}

void CorpusInit::setSizeOfCorpus(int sizeOfCorpus)
{
    sizeOfCorpus_ = sizeOfCorpus;
}

void CorpusInit::setArgc(int argc)
{
    argc_ = argc;
}

char** CorpusInit::argv()
{
    return argv_;
}

int CorpusInit::sizeOfCorpus()
{
    return sizeOfCorpus_;
}

int CorpusInit::argc()
{
    return argc_;
}

std::vector<std::string> CorpusInit::getListOfFiles()
{
    std::vector<std::string> result; 
    std::string path = argv()[1];
    int counter = 0; 

    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        result.push_back(entry.path().filename());
        counter++;
    }

    return result;
}

void CorpusInit::copyInputFiles(int count, SampleProcessing sampleProcessing, std::string fileName)
{
    std::string data = sampleProcessing.getBytes(fileName, argv()[1]);

    for(int i = 0; i < count; i++)
    {
        sampleProcessing.createNew(data, std::to_string(i + 1) + "-copy-of-" + fileName, argv()[1]);
    }
}

std::queue<Sample> CorpusInit::createNew()
{
    std::queue<Sample> result;
    std::vector<std::string> fileNames = getListOfFiles();
    SampleProcessing sampleProcessingForInit;
        
    if(fileNames.size() == 0)
    {
        std::cout << "You must create some input files in dir for inputs" << std::endl;
        exit(EXIT_FAILURE);
    } 
    else if(fileNames.size() < sizeOfCorpus())
    {
        copyInputFiles(sizeOfCorpus() - fileNames.size(), sampleProcessingForInit, fileNames.at(0));
        fileNames = getListOfFiles();
    }

    for(int i = 0; i < sizeOfCorpus(); i++)
    {
        
        SampleProcessing sampleProcessing;
        Mutation mutation(argv()[2]);
        CrashesProcessing crashesProcessing(argv()[3]);
        CodeCoverage codeCoverage(fileNames.at(i), argv(), argc());
        // codeCoverage.start(); //TODO SPRAV TO V FUZZERY 
        Sample sample(sampleProcessing, codeCoverage, fileNames.at(i), crashesProcessing, mutation);
        result.push(sample);
    }

    return result;
}