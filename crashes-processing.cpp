#include "crashes-processing.hpp"

#include <fstream>
#include <iostream>
#include "logger.hpp"

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

CrashesProcessing::CrashesProcessing(std::string dirForCrashFiles)
{
	setDirForCrashFiles(dirForCrashFiles);
}

void CrashesProcessing::createNew(std::string crash, std::string fileName)
{
	std::string fullName = dirForCrashFiles() + "/" + fileName;
	LOG_INFO("Start of new %s crash creating", fullName.c_str());
	std::ofstream fout(fullName, std::ios::binary);

	if (fout.is_open())
	{
		fout.seekp(0, std::ios::beg);
		size_t previousPos = fout.tellp();
		fout.write(&crash[0], crash.size());
		size_t writtenBytes = fout.tellp() - previousPos; 
		LOG_DEBUG("%d bytes from %d was written", writtenBytes, crash.size());
	}
	else
	{
		LOG_ERROR("Failed to create %s", fullName.c_str());
		exit(EXIT_FAILURE);
	}
	LOG_INFO("Crash was created successfully");
}