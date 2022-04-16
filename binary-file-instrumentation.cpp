#include "binary-file-instrumentation.hpp"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <sys/wait.h>
#include <stdio.h>

BinaryFileInstrumentation::BinaryFileInstrumentation()
{
}

void BinaryFileInstrumentation::setData(std::vector<uint64_t> data)
{
    data_ = data;
}

std::vector<uint64_t> BinaryFileInstrumentation::data()
{
    return data_;
}

void BinaryFileInstrumentation::setSaveOut(int saveOut)
{
    saveOut_ = saveOut;
}

int BinaryFileInstrumentation::saveOut()
{
    return saveOut_;
}

bool BinaryFileInstrumentation::createMyFifo()
{    
    if(mkfifo("myfifo", 0777) == -1){
        perror("The following mkfifo(""myfifo"", 0777) error occurred");
        exit(EXIT_FAILURE);
    }

    return true;
}

bool BinaryFileInstrumentation::runE9PatchTool()
{
    switch(fork())
    {
        case -1:
        {
            perror("The following fork() error occurred");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            //child
            if(execl("./e9patch/e9tool", "./e9patch/e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation.out", "example.out", (char*) NULL) == -1)
            {
                perror("The following (""./e9patch/e9tool"", ""./e9patch/e9tool"", ""-M"", ""asm=/j.*/"", ""-P"", ""entry(addr)@instrumentation.out"", ""example.out"", (char*) NULL) error occurred");
                exit(EXIT_FAILURE);
            }
        }
        default:
        {
            //parent
            if(wait(NULL) == -1)
            {
                perror("The following wait(NULL) error occurred");
                exit(EXIT_FAILURE);
            }

            runInstrumentedBinaryFile();
        }
    }

    return true;
}

bool BinaryFileInstrumentation::runInstrumentedBinaryFile()
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

    return true;
}

bool BinaryFileInstrumentation::redirectSTDOut()
{
    setSaveOut(dup(STDOUT_FILENO));
    if(saveOut() == -1)
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

    return true;
}

std::vector<uint64_t> BinaryFileInstrumentation::saveTheReachedBlocks()
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
        result.push_back(address);
    }

    if(close(fD) == -1)
    {
        perror("Error in close(fD) occurred");
        exit(EXIT_FAILURE);
    }

    //closeMyFifo();

    return result;
}

bool BinaryFileInstrumentation::closeMyFifo()
{
    if(unlink("myfifo") == -1)
    {
        perror("Error in unlink(""myfifo"") occurred");
        exit(EXIT_FAILURE);
    }

    return true;
}

bool BinaryFileInstrumentation::start()
{
    createMyFifo();
    runE9PatchTool();

    return true;
}

void BinaryFileInstrumentation::printData()
{
    for (uint64_t d : data())
    {
        std::cout << d << std::endl;
    }
}