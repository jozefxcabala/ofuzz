#include "crashes-processing.hpp"

#include <fstream>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <memory>
#include <stdexcept>
#include <array>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "logger.hpp"

void CrashesProcessing::setId(int id)
{
	id_ = id;
}

int CrashesProcessing::id()
{
	return id_;
}

std::string CrashesProcessing::getDate()
{
    LOG_INFO(id(), "Getting timestamp");
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");

    LOG_INFO(id(), "Timestamp: %s was got successfully", oss.str().c_str());

    return oss.str();
}

std::string CrashesProcessing::dirForCrashFiles()
{
	return difForCrashFiles_;
}

void CrashesProcessing::setDirForCrashFiles(std::string dirForCrashFiles)
{
	difForCrashFiles_ = dirForCrashFiles;
}

CrashesProcessing::CrashesProcessing()
{
}

CrashesProcessing::CrashesProcessing(std::string dirForCrashFiles, char** argv, int argc, std::string inputFile, int id)
{
	setDirForCrashFiles(dirForCrashFiles);
	setArgc(argc);
	setArgv(argv);
	setInputFile(inputFile);
	setId(id);
}

void CrashesProcessing::createNew(std::string data, std::string fileName)
{
	std::string fullName = dirForCrashFiles() + "/" + fileName;
	LOG_INFO(id(), "Start of new %s crash creating", fullName.c_str());
	std::ofstream fout(fullName, std::ios::binary);

	if (fout.is_open())
	{
		fout.seekp(0, std::ios::beg);
		size_t previousPos = fout.tellp();
		fout.write(&data[0], data.size());
		size_t writtenBytes = fout.tellp() - previousPos; 
		LOG_DEBUG(id(), "%d bytes from %d was written", writtenBytes, data.size());
	}
	else
	{
		LOG_ERROR(id(), "Failed to create %s", fullName.c_str());
		exit(EXIT_FAILURE);
	}
	LOG_INFO(id(), "Crash was created successfully");
}

void CrashesProcessing::start(std::string data)
{
	LOG_INFO(id(), "Start of crash processing");
	checkForCrash(data);
	LOG_INFO(id(), "Crash processing ended successfully");
}

void CrashesProcessing::setArgv(char** argv)
{
    argv_ = argv;
}

void CrashesProcessing::setInputFile(std::string inputFile)
{
    inputFile_ = inputFile;
}

void CrashesProcessing::setArgc(int argc)
{
    argc_ = argc;
}

char** CrashesProcessing::argv()
{
    return argv_;
}

std::string CrashesProcessing::inputFile()
{
    return inputFile_;
}

int CrashesProcessing::argc()
{
    return argc_;
}

void CrashesProcessing::checkForCrash(std::string data){
	std::string targetApplication(argv()[2]);

	LOG_INFO(id(), "Start of checking for crash from target application: %s", targetApplication.c_str());

	LOG_DEBUG(id(), "Start of build cmd for run target application: %s", targetApplication.c_str());

    const char* file = (std::string("./") + std::string(argv()[2])).c_str();
    
	pid_t child_pid;
    int child_status;

    child_pid = fork();
    if (child_pid == 0) {
        
        // this means we're the child process
        int fd = open("/dev/null", O_WRONLY);

        // dup both stdout and stderr and send them to /dev/null
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
        

        
        if(argc() == 3)
        {
            if(execl(file, file, inputFile().c_str(), (char*) NULL) == -1) // TODO zmen inputFile aby sa dal poslat argumentom
            {
                LOG_ERROR(id(), "Error in execl(""./a.out"", ""./a.out"", inputFile(), (char*) NULL) occurred: %s", std::strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        else if(argc() == 4)
        {
            if(execl(file, file, argv()[3], inputFile().c_str(), (char*) NULL) == -1) // TODO zmen inputFile aby sa dal poslat argumentom
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
        //execvp(file, arg);
        // shouldn't return, if it does, we have an error with the command
        LOG_ERROR(id(), "[!] Unknown command for execvp, exiting...\n");
        exit(1);
    }
    else {
        // this is run by the parent process
        do {
            pid_t tpid = waitpid(child_pid, &child_status, WUNTRACED |
             WCONTINUED);
            if (tpid == -1) {
                LOG_ERROR(id(), "[!] Waitpid failed!\n");
                perror("waitpid");
            }
            if (WIFEXITED(child_status)) {
                //LOG_DEBUG("WIFEXITED: Exit Status: %d\n", WEXITSTATUS(child_status));
            } else if (WIFSIGNALED(child_status)) {
                //crashes++;
                int exit_status = WTERMSIG(child_status);
                //LOG_DEBUG("\r[>] Crashes: %d", crashes);
                fflush(stdout);
                char command[50];
                sprintf(command, "cp mutated.jpeg ccrashes/%d.%d", id(), 
                exit_status);
                //system(command);
				std::string suffix = inputFile().substr(inputFile().find("/") + 1);
				createNew(data, "exit-status-" + std::to_string(exit_status) + "-" + getDate() + "-" + suffix);
            } else if (WIFSTOPPED(child_status)) {
                LOG_DEBUG(id(), "WIFSTOPPED: Exit Status: %d\n", WSTOPSIG(child_status));
            } else if (WIFCONTINUED(child_status)) {
                LOG_DEBUG(id(), "WIFCONTINUED: Exit Status: Continued.\n");
            }
        } while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));
    }

	LOG_INFO(id(), "Checking for crash from target application: %s ended successfully", targetApplication.c_str());
}