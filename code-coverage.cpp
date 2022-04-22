#include "code-coverage.hpp"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <sys/wait.h>
#include <stdio.h>
#include <algorithm>

CodeCoverage::CodeCoverage()
{

}

void CodeCoverage::setData(std::vector<uint64_t> data)
{
    data_ = data;
}

void CodeCoverage::setCoverage(int coverage)
{
    coverage_ = coverage;
}

std::vector<uint64_t> CodeCoverage::data()
{
    return data_;
}

int CodeCoverage::coverage()
{
    return coverage_;
}


void CodeCoverage::createMyFifo()
{    
    if(mkfifo("myfifo", 0777) == -1){
        perror("The following mkfifo(""myfifo"", 0777) error occurred");
        exit(EXIT_FAILURE);
    }
}

int CodeCoverage::getNumberOfBlocks()
{
    return data().size();
}

int CodeCoverage::calculateCoverage()
{
    createMyFifo();
    wait(NULL);
    runInstrumentedBinaryFile();
    setCoverage(getNumberOfBlocks());
}


void CodeCoverage::runInstrumentedBinaryFile()
{
    switch(fork()){
        case -1:
        {
            perror("The following fork() error occurred");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            redirectSTDOut();

            if(execl("./a.out", "./a.out", (char*) NULL) == -1)
            {
                perror("Error in execl(""./a.out"", ""./a.out"", (char*) NULL) occurred");
                exit(EXIT_FAILURE);
            }
        }
        default:
        {
            setData(saveTheReachedBlocks());
            closeMyFifo();
        }
    }
}

void CodeCoverage::redirectSTDOut()
{
    int saveOut = dup(STDOUT_FILENO);

    if(saveOut == -1)
    {
        perror("Error in dup(STDOUT_FILENO) occurred");
        exit(EXIT_FAILURE);
    }

    int devNull = open("/dev/null", O_WRONLY);
    if(devNull == -1)
    {
        perror("Error in open('/dev/null',0) occurred");
        exit(EXIT_FAILURE);
    }

    int dup2Result = dup2(devNull, STDOUT_FILENO);
    if(dup2Result == -1)
    {
        perror("Error in dup2(devNull, STDOUT_FILENO) occurred");
        exit(EXIT_FAILURE);
    }
}

bool CodeCoverage::checkForUniqueValue(std::vector<uint64_t> data, uint64_t value)
{
    if (std::find(data.begin(), data.end(), value) != data.end()) {
        return false;
    }
    else {
        return true;
    }
}

std::vector<uint64_t> CodeCoverage::saveTheReachedBlocks()
{
    std::vector<uint64_t> result;
    uint64_t address{0};

    int fD = open("myfifo", O_RDONLY);

    if(fD == -1)
    {
        perror("Error in open(""myfifo"", O_RDONLY) occurred");
        exit(EXIT_FAILURE);
    }

    while(read(fD, &address, sizeof(uint64_t)))
    {
        if(checkForUniqueValue(result, address))
        {
            result.push_back(address);
        }      
    }

    if(close(fD) == -1)
    {
        perror("Error in close(fD) occurred");
        exit(EXIT_FAILURE);
    }

    //closeMyFifo();

    return result;
}

void CodeCoverage::closeMyFifo()
{
    if(unlink("myfifo") == -1)
    {
        perror("Error in unlink(""myfifo"") occurred");
        exit(EXIT_FAILURE);
    }
}

void CodeCoverage::start()
{
    calculateCoverage();

}

void CodeCoverage::printData()
{
    std::cout << "Number of reached blocks: " << coverage() << std::endl;
    std::cout << "Reached blocks: " << std::endl;

    for (uint64_t d : data())
    {
        std::cout << d << std::endl;
    }
}