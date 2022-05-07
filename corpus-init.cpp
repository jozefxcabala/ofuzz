#include "corpus-init.hpp"
#include "sample-processing.hpp"
#include "mutation.hpp"
#include "crashes-processing.hpp"

#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>

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
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "-%d-%m-%Y-%H-%M-%S");

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

	if(stat((dirName + getDate()).c_str(), &info) != 0)
	{
		if(std::filesystem::create_directories(dirName + getDate()) == false)
		{
			std::cout << "creat directory: " << dirName + getDate() << " dont work" << std::endl;
			exit(EXIT_FAILURE); 
		}
	}

    return dirName + getDate();
}

std::vector<Sample> CorpusInit::start()
{
    struct stat info;

    if(stat(argv()[1], &info) != 0)
    {
        perror("input_file dont exist");
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
        sampleProcessing.createNew(data, listOfFiles.at(i), dirForMutations());
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
        Mutation mutation(dirForMutations());
        CrashesProcessing crashesProcessing(dirForCrashes());
        CodeCoverage codeCoverage(fileNames.at(i), argv(), argc());
        Sample sample(sampleProcessing, codeCoverage, fileNames.at(i), crashesProcessing, mutation);
        result.push_back(sample);
    }

    return result;
}