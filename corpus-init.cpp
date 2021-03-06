#include "corpus-init.hpp"
#include "sample-processing.hpp"
#include "mutation.hpp"
#include "crashes-processing.hpp"
#include "logger.hpp"

#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>
#include <cstring>

CorpusInit::CorpusInit()
{
}

CorpusInit::CorpusInit(char** argv, int sizeOfCorpus, int argc)
{   
    setArgv(argv);
    setSizeOfCorpus(sizeOfCorpus);
    setArgc(argc);
}

std::string CorpusInit::getDate()
{
    LOG_INFO(6, "Getting timestamp");
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");

    LOG_INFO(6, "Timestamp: %s was got successfully", oss.str().c_str());

    return oss.str();
}

std::string CorpusInit::dirForCrashes()
{
    return dirForCrashes_;
}

std::string CorpusInit::dirForMutations()
{
    return dirForMutations_;
}

void CorpusInit::setDirForCrashes(std::string dir)
{
    dirForCrashes_ = dir;
}

void CorpusInit::setDirForMutations(std::string dir)
{
    dirForMutations_ = dir;
}

std::string CorpusInit::createDir(std::string dirName)
{
    struct stat info;
    std::string fullDirName = dirName + "-" + getDate().c_str();
    LOG_INFO(6, "Start creating dir %s", fullDirName.c_str());

	if(stat(fullDirName.c_str(), &info) != 0)
	{
        LOG_DEBUG(6, "Dir %s does not exist, create dir", fullDirName.c_str());
		if(std::filesystem::create_directories(fullDirName) == false)
		{
            LOG_ERROR(6, "Creating dir %s does not work: %s", fullDirName.c_str(), std::strerror(errno));
			exit(EXIT_FAILURE); 
		}
	}

    LOG_INFO(6, "Dir %s was created successfully", fullDirName.c_str());

    return fullDirName;
}

std::vector<Sample> CorpusInit::start()
{
    LOG_INFO(6, "Start of CorpusInit");
    struct stat info;

    LOG_DEBUG(6, "Check if input file exist");
    if(stat(argv()[1], &info) != 0)
    {
        LOG_ERROR(6, std::strerror(errno));
        exit(EXIT_FAILURE);
    }

    setDirForMutations(createDir("mutations-file"));
    setDirForCrashes(createDir("crashes-file"));
    
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
    LOG_INFO(6, "Start of getLisOfFiles in input_file dir");
    std::vector<std::string> result; 
    std::string path = argv()[1];
    int counter = 0; 

    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        LOG_DEBUG(6, "%s is pushing to vector", entry.path().filename().c_str());
        result.push_back(entry.path().filename());
        LOG_DEBUG(6, "%s was pushed to vector successfully", entry.path().filename().c_str());
        counter++;
    }

    LOG_INFO(6, "getListOfFiles was ended successfully");

    return result;
}

void CorpusInit::copyInputFiles(int count, SampleProcessing sampleProcessing, std::string fileName)
{
    LOG_INFO(6, "Start of copyInputFiles");
    std::string data = sampleProcessing.getBytes(fileName, argv()[1]);

    for(int i = 0; i < count; i++)
    {
        sampleProcessing.createNew(data, std::to_string(i + 1) + "-copy-of-" + fileName, argv()[1]);
    }

    LOG_INFO(6, "copyInputFiles was ended successfully");
}

void CorpusInit::createMutationFiles(int count, SampleProcessing sampleProcessing, std::vector<std::string> listOfFiles)
{
    LOG_INFO(6, "Start of createMutationFiles");
    std::string data;
    std::string suffix;

    for(int i = 0; i < count; i++)
    {
        data = sampleProcessing.getBytes(listOfFiles.at(i), argv()[1]);
        sampleProcessing.createNew(data, listOfFiles.at(i), dirForMutations());
    }
    int posOfDot = listOfFiles.at(0).find(".");

    if(posOfDot == -1)
    {
        suffix = "";
    }
    else
    {
        suffix = listOfFiles.at(0).substr(listOfFiles.at(0).find("."));
    }

    sampleProcessing.createNew(data, "best-coverage", dirForMutations());

    LOG_INFO(6, "createMutationFiles was ended successfully");
}

std::vector<Sample> CorpusInit::createNew()
{
    LOG_INFO(6, "Start of createNew function to create corpus for fuzzer");
    std::vector<Sample> result;
    std::vector<std::string> fileNames = getListOfFiles();
    SampleProcessing sampleProcessingForInit(6);
        
    if(fileNames.size() == 0)
    {
        LOG_ERROR(6, "You must create some input files in %s", argv()[1]);
        exit(EXIT_FAILURE);
    } 
    else if(fileNames.size() < sizeOfCorpus())
    {
        LOG_INFO(6, "Size of input files is lower than size of corpus, i am creating copy of input files");
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
        LOG_DEBUG(6, "Sample is initializing");
        SampleProcessing sampleProcessing(i);
        Mutation mutation(dirForMutations(), i);
        CrashesProcessing crashesProcessing(dirForCrashes(), argv(), argc(), dirForMutations() + "/" + fileNames.at(i), i);
        CodeCoverage codeCoverage(dirForMutations() + "/" + fileNames.at(i), argv(), argc(), i);
        Sample sample(sampleProcessing, codeCoverage, fileNames.at(i), crashesProcessing, mutation);
        
        LOG_DEBUG(6, "Sample of file %s, is pushing to vector", sample.fileName().c_str());
        result.push_back(sample);
        LOG_DEBUG(6, "Sample of file %s, was pushed to vector successfully", sample.fileName().c_str());
    }

    LOG_INFO(6, "createNew corpus for fuzzer ended successfully");

    return result;
}