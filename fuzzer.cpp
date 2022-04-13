// #include <iostream>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <vector>
// #include <sstream>
// #include <sys/wait.h>

// int main(int argc, char* argv[])
// {
//     int fd;
//     char myfifo [] = "myfifo";
//     std::vector<uint64_t> data;
//     int save_out;

//     if(mkfifo(myfifo, 0777) == -1){
//         printf("pipe sa neotvorila");
//         return -1;
//     }
    
//     switch(fork()){
//         case -1:
//         {
//             printf("fork nefunguje normalne");
//             return -1;
//         }
//         case 0:
//         {
//             //child
//             execl("./e9patch/e9tool", "./e9patch/e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation.out", "example.out", (char*) NULL);
//         }
//         default:
//         {
//             //parent
//             wait(NULL);
//             switch(fork()){
//                 case -1:
//                 {
//                     printf("fork nefunguje normalne");
//                     return -1;
//                 }
//                 case 0:
//                 {
//                     save_out = dup(STDOUT_FILENO);
//                     if(save_out == -1)
//                     {
//                         fprintf(stderr,"Error in dup(STDOUT_FILENO)\n");
//                         exit(EXIT_FAILURE);
//                     }

//                     int devNull = open("/dev/null",O_WRONLY);
//                     if(devNull == -1)
//                     {
//                         fprintf(stderr,"Error in open('/dev/null',0)\n");
//                         exit(EXIT_FAILURE);
//                     }

//                     int dup2Result = dup2(devNull, STDOUT_FILENO);
//                     if(dup2Result == -1)
//                     {
//                         fprintf(stderr,"Error in dup2(devNull, STDOUT_FILENO)\n");
//                         exit(EXIT_FAILURE);
//                     }

//                     execl("./a.out", "./a.out", (char*) NULL);
//                 }
//                 default:
//                 {
//                     uint64_t address{0};
//                     fd = open(myfifo, O_RDONLY);
//                     while(read(fd, &address, sizeof(uint64_t))){
//                         std::cout << address << std::endl;
//                         data.push_back(address);
//                     }
//                     close(fd);   
//                     unlink(myfifo);
//                 }
//             }
//         }
//     }
//     return 0;
// }

#include "binary-file-instrumentation.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <sys/wait.h>

BinaryFileInstrumentation::BinaryFileInstrumentation()
{
    setMyFifo("myFifo");
    std::cout << "BinaryFileInstrumentation was created!" << std::endl;
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

void BinaryFileInstrumentation::setMyFifo(std::string myFifo)
{
    myFifo_ = myFifo;
}

std::string BinaryFileInstrumentation::myFifo()
{
    return myFifo_;
}

bool BinaryFileInstrumentation::createMyFifo()
{
    const char * fifo = myFifo().c_str();
    std::cout << fifo << std::endl;
    
    if(mkfifo(fifo, 0777) == -1){
        std::cerr << "The pipe did not open properly" << std::endl;
        return false;
    }

    return true;
}

bool BinaryFileInstrumentation::runE9PatchTool()
{
    switch(fork())
    {
        case -1:
        {
            std::cerr << "The fork did not work properly" << std::endl;
            return false;
        }
        case 0:
        {
            //child - TODO osetri execl
            execl("./e9patch/e9tool", "./e9patch/e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation.out", "example.out", (char*) NULL);
        }
        default:
        {
            //parent
            wait(NULL);

            if(runInstrumentedBinaryFile())
            {
                return true;
            } 
            else 
            {
                std::cerr << "runInstrumentedBinaryFile did not work properly" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool BinaryFileInstrumentation::runInstrumentedBinaryFile()
{
    switch(fork()){
        case -1:
        {
            std::cerr << "The fork did not work properly" << std::endl;
            return false;
        }
        case 0:
        {
            if(redirectSTDOut())
            {

                execl("./a.out", "./a.out", (char*) NULL);
            } 
            else 
            {
                std::cerr << "redirectSTDOut did not work properly" << std::endl;
                return false;
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
        std::cerr << "Error in dup(STDOUT_FILENO)" << std::endl;
        return false;
    }

    int devNull = open("/dev/null", O_WRONLY);
    if(devNull == -1)
    {
        std::cerr << "Error in open('/dev/null',0)" << std::endl;
        return false;
    }

    int dup2Result = dup2(devNull, STDOUT_FILENO);
    if(dup2Result == -1)
    {
        std::cerr << "Error in dup2(devNull, STDOUT_FILENO)" << std::endl;
        return false;
    }

    return true;
}

std::vector<uint64_t> BinaryFileInstrumentation::saveTheReachedBlocks()
{
    std::vector<uint64_t> result;
    uint64_t address{0};

    int fD = open(myFifo().c_str(), O_RDONLY);
    while(read(fD, &address, sizeof(uint64_t)))
    {
        result.push_back(address);
    }

    close(fD);
    closeMyFifo();

    return result;
}

bool BinaryFileInstrumentation::closeMyFifo()
{
    unlink(myFifo().c_str());
    return true;
}

bool BinaryFileInstrumentation::start()
{
    createMyFifo();
    if(runE9PatchTool())
    {
        return true;
    } 
    else 
    {
        std::cout << "runE9PatchTool did not work properly" << std::endl;
        return false;
    }
}

int main()
{
    BinaryFileInstrumentation binaryFileInstrumentation;
    std::cout << binaryFileInstrumentation.start() << std::endl;
    return 0;
}