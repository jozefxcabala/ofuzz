#include "corpus-init.hpp"
#include "sample-processing.hpp"
#include "mutation.hpp"
#include "crashes-processing.hpp"

#include <filesystem>
#include <iostream>
#include <sys/stat.h>

CorpusInit::CorpusInit()
{

}

CorpusInit::CorpusInit(char** argv, int sizeOfCorpus, int argc)
{
    setArgv(argv);
    setSizeOfCorpus(sizeOfCorpus);
    setArgc(argc);
}

void CorpusInit::createDir(std::string dirName)
{
    struct stat info;

	if(stat(dirName.c_str(), &info) != 0)
	{
		if(std::filesystem::create_directories(dirName) == false)
		{
			std::cout << "creat directory: " << dirName << " dont work" << std::endl;
			exit(EXIT_FAILURE); 
		}
	}
}

std::vector<Sample> CorpusInit::start()
{
    createDir(argv()[2]);
    createDir(argv()[3]);
    
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

void CorpusInit::createMutationFiles(int count, SampleProcessing sampleProcessing, std::vector<std::string> listOfFiles)
{
    std::string data;

    for(int i = 0; i < count; i++)
    {
        data = sampleProcessing.getBytes(listOfFiles.at(i), argv()[1]);
        sampleProcessing.createNew(data, listOfFiles.at(i), argv()[2]);
    }
}

std::vector<Sample> CorpusInit::createNew()
{
    std::vector<Sample> result;
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
        createMutationFiles(sizeOfCorpus(), sampleProcessingForInit, fileNames);
    }
    else
    {
        createMutationFiles(sizeOfCorpus(), sampleProcessingForInit, fileNames);
    }

    for(int i = 0; i < sizeOfCorpus(); i++)
    {
        
        SampleProcessing sampleProcessing;
        Mutation mutation(argv()[2]);
        CrashesProcessing crashesProcessing(argv()[3]);
        CodeCoverage codeCoverage(fileNames.at(i), argv(), argc());
        Sample sample(sampleProcessing, codeCoverage, fileNames.at(i), crashesProcessing, mutation);
        result.push_back(sample);
    }

    return result;
}