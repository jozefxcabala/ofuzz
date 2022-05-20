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

std::string CrashesProcessing::getOutput()
{
	std::string output;
	FILE * stream;
	std::array<char, 128> buffer;
	std::string cmd = "./";
	std::string targetApplication(argv()[2]);

	LOG_INFO(id(), "Start of getting output from target application: %s", targetApplication.c_str());

	LOG_DEBUG(id(), "Start of build cmd for run target application: %s", targetApplication.c_str());
	cmd.append(targetApplication);
	cmd.append(" ");

	if(argc() == 4)
	{
		cmd.append(argv()[2]);
		cmd.append(" ");
		cmd.append(inputFile());
	} 
	else if (argc() == 3) 
	{
		cmd.append(inputFile());
	} 
	else
	{
		LOG_ERROR(id(), "Not supported number of arguments");
        exit(EXIT_FAILURE);
	}

	LOG_DEBUG(id(), "Build of cmd for run target application: %s, ended successfully. cmd:  %s", targetApplication.c_str(), cmd.c_str());
 
	LOG_DEBUG(id(), "Star of running target application: %s, with potential crashable input file", targetApplication.c_str());
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) 
	{
		LOG_ERROR(id(), "Pipe is null");
        exit(EXIT_FAILURE);
    }
	LOG_DEBUG(id(), "Running of target application: %s, with potential crashable input file ended successfully", targetApplication.c_str());

	LOG_DEBUG(id(), "Start of building output from target application: %s", targetApplication.c_str());
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        output += buffer.data();
    }
	LOG_DEBUG(id(), "Build of output from target application: %s, ended successfully", targetApplication.c_str());

	output = "Segmentation"; // TODO toto tu je preto lebo inak to nefunguje, popros peta nech nato pozrie

	LOG_INFO(id(), "Getting output from target application: %s, ended successfully", targetApplication.c_str());
    return output;
}

void CrashesProcessing::checkForCrash(std::string output, std::string data)
{
	LOG_INFO(id(), "Start of checking for potential crash");
	std::string segfault = "Segmentation";
	std::string floating_point = "Floating";

	std::size_t pos1 = output.find(segfault);
	std::size_t pos2 = output.find(floating_point);

	std::string suffix = inputFile().substr(inputFile().find("/") + 1);

	if (pos1 != -1)
	{
		LOG_DEBUG(id(), "There is segmentation fault!");
		createNew(data, "segmentation-fault-" + getDate() + "-" + suffix);
	}
	else if (pos2 != -1)
	{
		LOG_DEBUG(id(), "There is floating point fault!");
		createNew(data, "floating-point-" + getDate() + "-" + suffix);
	}

	LOG_INFO(id(), "Checking for potential crash ended successfully");
}

void CrashesProcessing::start(std::string data)
{
	LOG_INFO(id(), "Start of crash processing");
	std::string output = getOutput();
	checkForCrash(output, data);
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
