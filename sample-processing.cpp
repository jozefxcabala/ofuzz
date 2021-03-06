#include "sample-processing.hpp"

#include <fstream>
#include <iostream>
#include "logger.hpp"
#include <cstring>

SampleProcessing::SampleProcessing()
{
}

SampleProcessing::SampleProcessing(int id)
{
	setId(id);
}

void SampleProcessing::setId(int id)
{
	id_ = id;
}

int SampleProcessing::id()
{
	return id_;
}

void SampleProcessing::createNew(std::string data, std::string fileName, std::string dirName)
{
	std::string fullName = dirName + "/" + fileName;
	LOG_INFO(id(), "Creating of %s", fullName.c_str());
	std::ofstream fout(fullName, std::ios::binary);

	if (fout.is_open())
	{
		LOG_DEBUG(id(), "Trying write %d bytes", data.size());
		fout.seekp(0, std::ios::beg);
		size_t previousPos = fout.tellp();
		fout.write(&data[0], data.size());
		size_t wrritenBytes = fout.tellp() - previousPos; 
		LOG_DEBUG(id(), "%d bytes was written", wrritenBytes);
	}
	else
	{
		LOG_ERROR(id(), "Failed to create %s: %s", fullName.c_str(), std::strerror(errno));
		exit(EXIT_FAILURE);
	}
	LOG_INFO(id(), "%s was created successfully", fullName.c_str());
}

std::string SampleProcessing::getBytes(std::string fileName, std::string dirName)
{
	std::string fullName = dirName + "/" + fileName;
	LOG_INFO(id(), "Start getting bytes from %s", fullName.c_str());
	std::ifstream fin(fullName, std::ios::binary);

	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		std::string data;
		data.resize(fin.tellg());
		fin.seekg(0, std::ios::beg);
		fin.read(&data[0], data.size());
		int countOfReadBytes = fin.gcount();
		LOG_DEBUG(id(), "%d bytes was read", countOfReadBytes);

		return data;
	}

	else
	{
		LOG_ERROR(id(), "Failed to open %s: %s", fullName.c_str(), std::strerror(errno));
		exit(EXIT_FAILURE);
	}

}