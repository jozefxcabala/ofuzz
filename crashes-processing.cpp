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
#include <chrono>
#include <sstream>
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

bool CrashesProcessing::start(std::string data)
{
	LOG_INFO(id(), "Start of crash processing");
	bool status = checkForCrash(data);
	LOG_INFO(id(), "Crash processing ended successfully");

    return status;
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

bool CrashesProcessing::checkForCrash(std::string data){
	std::string targetApplication(argv()[2]);

	LOG_INFO(id(), "Start of checking for crash from target application: %s", targetApplication.c_str());

    std::string file = std::string("./") + std::string(argv()[2]);
	pid_t child_pid;
    int child_status;
    int status = false;

    child_pid = fork();
    if (child_pid == 0) {
        
        // this means we're the child process
        int fd = open("/dev/null", O_WRONLY);

        // dup both stdout and stderr and send them to /dev/null
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
        

        LOG_INFO(id(), "Running application for finding crash");
        if(argc() == 3)
        {
            if(execl(file.c_str(), file.c_str(), inputFile().c_str(), (char*) NULL) == -1) // TODO zmen inputFile aby sa dal poslat argumentom
            {
                LOG_ERROR(id(), "Error in execl(""./a.out"", ""./a.out"", inputFile(), (char*) NULL) occurred: %s", std::strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        else if(argc() == 4)
        {
            if(execl(file.c_str(), file.c_str(), argv()[3], inputFile().c_str(), (char*) NULL) == -1) // TODO zmen inputFile aby sa dal poslat argumentom
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

        LOG_ERROR(id(), "[!] Unknown command for execvp, exiting...\n");
        exit(EXIT_FAILURE);
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
                LOG_DEBUG(id(), "App %s exited successfully", targetApplication.c_str());
            } else if (WIFSIGNALED(child_status)) {
                int exit_status = WTERMSIG(child_status);
                fflush(stdout);
				std::string suffix = inputFile().substr(inputFile().find("/") + 1);
                std::string crashName = "exit-status-" + std::to_string(exit_status) + "-" + getDate() + "-" + suffix;
                LOG_APP(id(), "%s was created", crashName.c_str());
				createNew(data, crashName);
                status = true;
            } else if (WIFSTOPPED(child_status)) {
                LOG_DEBUG(id(), "WIFSTOPPED: Exit Status: %d\n", WSTOPSIG(child_status));
            } else if (WIFCONTINUED(child_status)) {
                LOG_DEBUG(id(), "WIFCONTINUED: Exit Status: Continued.\n");
            }
        } while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));
    }

	LOG_INFO(id(), "Checking for crash from target application: %s ended successfully", targetApplication.c_str());

    return status;
}