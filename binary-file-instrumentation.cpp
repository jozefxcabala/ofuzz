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
    LOG_INFO("E9PatchTool was started");
    int childPid;

    switch(childPid = fork())
    {
        case -1:
        {
            LOG_ERROR("The following fork() error occurred: %s", std::strerror(errno));
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            LOG_INFO("Start of instrumentation");
            if(execl("./e9patch/e9tool", "./e9patch/e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation.out", targetApplication().c_str(), (char*) NULL) == -1)
            {
                LOG_ERROR("The following ""./e9patch/e9tool"", ""./e9patch/e9tool"", ""-M"", ""asm=/j.*/"", ""-P"", ""entry(addr)@instrumentation.out"", ""%s"", (char*) NULL) error occurred", targetApplication().c_str());
                exit(EXIT_FAILURE);
            }
        }
        default:
        {
            int returnStatus;    
            waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.

            if (returnStatus == 1)      
            {
                LOG_ERROR("The child process (binary instrumentation) terminated with an error: %s", std::strerror(errno));
                exit(EXIT_FAILURE);
            }
            LOG_INFO("Instrumentation ended successfully");
        }
    }

    LOG_INFO("E9PatchTool ended successfully");
}

void BinaryFileInstrumentation::start()
{
    LOG_INFO("Start binary file instrumentation");
    runE9PatchTool();
    LOG_INFO("Binary file instrumentation ended successfully");
}

