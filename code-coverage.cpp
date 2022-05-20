#include "code-coverage.hpp"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <sys/wait.h>
#include <stdio.h>
#include <algorithm>
#include "logger.hpp"
#include <cstring>

void CodeCoverage::setId(int id)
{
	id_ = id;
}

int CodeCoverage::id()
{
	return id_;
}

CodeCoverage::CodeCoverage()
{
}

CodeCoverage::CodeCoverage(std::string inputFile, char** argvA, int argcA, int id)
{
    setInputFile(inputFile);
    setArgv(argvA);
    setArgc(argcA);
    setCoverage(0);
    setId(id);
}

void CodeCoverage::setArgv(char** argv)
{
    argv_ = argv;
}

void CodeCoverage::setInputFile(std::string inputFile)
{
    inputFile_ = inputFile;
}

std::string CodeCoverage::inputFile()
{
    return inputFile_;
}

void CodeCoverage::setData(std::vector<uint64_t> data)
{
    data_ = data;
}

void CodeCoverage::setCoverage(int coverage)
{
    coverage_ = coverage;
}

void CodeCoverage::setArgc(int argc)
{
    argc_ = argc;
}

std::vector<uint64_t> CodeCoverage::data()
{
    return data_;
}

int CodeCoverage::coverage()
{
    return coverage_;
}

char** CodeCoverage::argv()
{
    return argv_;
}

int CodeCoverage::argc()
{
    return argc_;
}

void CodeCoverage::createMyFifo()
{    
    LOG_INFO(id(), "Start of creating myfifo");
    if(mkfifo("myfifo", 0777) == -1){
        LOG_ERROR(id(), "The followgin mkfifo(""myfifo"", 0777) error occured: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }
    LOG_INFO(id(), "myfifo was created successfully");
}

int CodeCoverage::getNumberOfBlocks()
{
    return data().size();
}

void CodeCoverage::calculateCoverage()
{
    LOG_INFO(id(), "Start of calculateCoverage");
    createMyFifo();
    wait(NULL);
    runInstrumentedBinaryFile();
    setCoverage(getNumberOfBlocks());
    LOG_INFO(id(), "calculateCoverage ended successfully");
}


void CodeCoverage::runInstrumentedBinaryFile()
{
    LOG_INFO(id(), "Start instrumentedBinaryFile");
    int childPid;
    switch(childPid = fork()){
        case -1:
        {
            LOG_ERROR(id(), "The following fork() error occurred: %s", std::strerror(errno));
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            //redirectSTDOut(); //tuto je problem preto to nebezi, nevieme preco to funguje na jednom vlakne, ale na viacerych nie ... TODO mozes skusit vyriesit neskor 

            LOG_DEBUG(id(), "Run target application with %s, num of arguments is %d", inputFile().c_str(), argc());
            if(argc() == 3)
            {
                if(execl("./a.out", "./a.out", inputFile().c_str(), (char*) NULL) == -1) // TODO zmen inputFile aby sa dal poslat argumentom
                {
                    LOG_ERROR(id(), "Error in execl(""./a.out"", ""./a.out"", inputFile(), (char*) NULL) occurred: %s", std::strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            else if(argc() == 4)
            {
                if(execl("./a.out", "./a.out", argv()[3], inputFile().c_str(), (char*) NULL) == -1) // TODO zmen inputFile aby sa dal poslat argumentom
                {
                    LOG_ERROR(id(), "Error in execl(""./a.out"", ""./a.out"", argv()[3], inputFile(), (char*) NULL) occurred: %s", std::strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                LOG_ERROR(id(), "Not supported number of arguments");
                exit(EXIT_FAILURE);
            }
        }
        default:
        {
            LOG_DEBUG(id(), "Waiting for end of instrumented target application");
            // int returnStatus;    
            // waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.

            // if (returnStatus == 1)      
            // {
            //     LOG_ERROR("The child process (running instrumented target application) terminated with an error: %s", std::strerror(errno));
            //     exit(EXIT_FAILURE);
            // }

            LOG_DEBUG(id(), "Instrumented target application was ended successfully");
            setData(saveTheReachedBlocks());
            closeMyFifo();
        }
    }
    LOG_INFO(id(), "Run binaryInstrumentedFile ended successfully");
}


void CodeCoverage::redirectSTDOut()
{
    LOG_INFO(id(), "Start of redirecting STDOut");
    int saveOut = dup(STDOUT_FILENO);

    if(saveOut == -1)
    {
        LOG_ERROR(id(), "Error in dup(STDOUT_FILENO) occurred: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }

    int devNull = open("/dev/null", O_WRONLY);
    if(devNull == -1)
    {
        LOG_ERROR(id(), "Error in open('/dev/null',0) occurred: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }

    int dup2Result = dup2(devNull, STDOUT_FILENO);
    if(dup2Result == -1)
    {
        LOG_ERROR(id(), "Error in dup2(devNull, STDOUT_FILENO) occurred: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }
    LOG_INFO(id(), "Redirecting STDOut ended successfully");
}

bool CodeCoverage::checkForUniqueValue(std::vector<uint64_t> data, uint64_t value)
{
    LOG_DEBUG(id(), "Checking if values are unique");
    if (std::find(data.begin(), data.end(), value) != data.end()) {
        LOG_DEBUG(id(), "value is not unique");
        return false;
    }
    else {
        LOG_DEBUG(id(), "value is unique");
        return true;
    }
}

std::vector<uint64_t> CodeCoverage::saveTheReachedBlocks()
{
    LOG_INFO(id(), "Start of saving the reached unique blocks");
    std::vector<uint64_t> result;
    uint64_t address{0};

    LOG_DEBUG(id(), "Opening of myfifo");
    int fD = open("myfifo", O_RDONLY);

    if(fD == -1)
    {
        LOG_ERROR(id(), "Error in open(""myfifo"", O_RDONLY) occurred: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }

    LOG_DEBUG(id(), "myfifo was opened successfully");
    LOG_DEBUG(id(), "Reading of reached addresses");
    while(read(fD, &address, sizeof(uint64_t)))
    {
        if(checkForUniqueValue(result, address))
        {
            LOG_DEBUG(id(), "Try push unique address to result");
            result.push_back(address);
            LOG_DEBUG(id(), "Unique address was pushed to vector successfully");
        }      
    }

    LOG_DEBUG(id(), "Closing myfifo");
    if(close(fD) == -1)
    {
        LOG_ERROR(id(), "Error in close(fD) occurred: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }
    LOG_DEBUG(id(), "myfifo was closed successfully");

    //closeMyFifo();

    LOG_INFO(id(), "Saving the reached unique block was successfully");

    return result;
}

void CodeCoverage::closeMyFifo()
{
    LOG_INFO(id(), "Start of closing myfifo");
    if(unlink("myfifo") == -1)
    {
        LOG_ERROR(id(), "Error in unlink(""myfifo"") occurred: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }
    LOG_INFO(id(), "myfifo was closed successfully");
}

void CodeCoverage::start()
{
    LOG_INFO(id(), "Start of CodeCoverage");
    calculateCoverage();
    LOG_INFO(id(), "CodeCoverage ended successfully");
}

void CodeCoverage::printData()
{
    LOG_INFO(id(), "Start printing data");
    std::cout << "Number of reached blocks: " << coverage() << std::endl;
    std::cout << "Reached blocks: " << std::endl;

    for (uint64_t d : data())
    {
        std::cout << d << std::endl;
    }

    LOG_INFO(id(), "Data was printed successfully");
}
