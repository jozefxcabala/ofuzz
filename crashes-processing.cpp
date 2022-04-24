#include "crashes-processing.hpp"

#include <fstream>
#include <iostream>

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
	std::ofstream fout(dirForCrashFiles() + "/" + fileName, std::ios::binary);

	if (fout.is_open())
	{
		fout.seekp(0, std::ios::beg);
		fout.write(&crash[0], crash.size());
	}
	else
	{
		std::cout << "Failed to create: " << (dirForCrashFiles() + "/" + fileName) << std::endl; //TODO podla toho ako to mam inde
		exit(1);
	}

}