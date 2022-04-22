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

void BinaryFileInstrumentation::runE9PatchTool()
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
        }
    }
}

void BinaryFileInstrumentation::start()
{
    runE9PatchTool();
}

