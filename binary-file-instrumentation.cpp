#include "binary-file-instrumentation.hpp"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <sys/wait.h>
#include <stdio.h>
#include "logger.hpp"
#include <cstring>

BinaryFileInstrumentation::BinaryFileInstrumentation()
{
}

BinaryFileInstrumentation::BinaryFileInstrumentation(std::string targetApplication)
{
    setTargetApplication(targetApplication);
}

void BinaryFileInstrumentation::setTargetApplication(std::string targetApplication)
{
    targetApplication_ = targetApplication;
}

std::string BinaryFileInstrumentation::targetApplication()
{
    return targetApplication_;
}

void BinaryFileInstrumentation::runE9PatchTool()
{
    LOG_INFO(6, "E9PatchTool was started");
    int childPid;

    switch(childPid = fork())
    {
        case -1:
        {
            LOG_ERROR(6, "The following fork() error occurred: %s", std::strerror(errno));
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            //asm=/j.*/ or asm=/cmp.*/ or asm=/ret.*/ or asm=/push.*/
            LOG_INFO(6, "Start of instrumentation");
            if(execl("./e9patch/e9tool", "./e9patch/e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation.out", targetApplication().c_str(), (char*) NULL) == -1)
            {
                LOG_ERROR(6, "The following ""./e9patch/e9tool"", ""./e9patch/e9tool"", ""-M"", ""asm=/j.*/"", ""-P"", ""entry(addr)@instrumentation.out"", ""%s"", (char*) NULL) error occurred", targetApplication().c_str());
                exit(EXIT_FAILURE);
            }
        }
        default:
        {
            int returnStatus;    
            waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.

            if (returnStatus == 1)      
            {
                LOG_ERROR(6, "The child process (binary instrumentation) terminated with an error: %s", std::strerror(errno));
                exit(EXIT_FAILURE);
            }
            LOG_INFO(6, "Instrumentation ended successfully");
        }
    }

    LOG_INFO(6, "E9PatchTool ended successfully");
}

void BinaryFileInstrumentation::start()
{
    LOG_INFO(6, "Start binary file instrumentation");
    runE9PatchTool();
    LOG_INFO(6, "Binary file instrumentation ended successfully");
}

