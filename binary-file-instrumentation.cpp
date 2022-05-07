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
    int childPid;

    switch(childPid = fork())
    {
        case -1:
        {
            perror("The following fork() error occurred");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            //child
            if(execl("./e9patch/e9tool", "./e9patch/e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation.out", targetApplication().c_str(), (char*) NULL) == -1)
            {
                perror(("The following (""./e9patch/e9tool"", ""./e9patch/e9tool"", ""-M"", ""asm=/j.*/"", ""-P"", ""entry(addr)@instrumentation.out"", """ + targetApplication() + """, (char*) NULL) error occurred").c_str());
                exit(EXIT_FAILURE);
            }
        }
        default:
        {
            int returnStatus;    
            waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.

            if (returnStatus == 1)      
            {
                perror("The child process (binary instrumentation) terminated with an error!.");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void BinaryFileInstrumentation::start()
{
    runE9PatchTool();
}

