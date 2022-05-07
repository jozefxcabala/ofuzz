#include "sample-processing.hpp"

#include <fstream>
#include <iostream>

SampleProcessing::SampleProcessing()
{

}

void SampleProcessing::createNew(std::string data, std::string fileName, std::string dirName)
{
	std::ofstream fout(dirName + "/" + fileName, std::ios::binary);

	if (fout.is_open())
	{
		//std::cout << "size is CREATE NEW: " << data.size() << " " << dirName << " " << fileName<< std::endl;
		fout.seekp(0, std::ios::beg);
		fout.write(&data[0], data.size());
	}
	else
	{
		std::cout << "Failed to create: " << (dirName + "/" + fileName) << std::endl; //TODO podla toho ako to mam inde
		exit(1);
	}

}

std::string SampleProcessing::getBytes(std::string fileName, std::string dirName)
{
	std::ifstream fin(dirName + "/" + fileName, std::ios::binary);

	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		std::string data;
		data.resize(fin.tellg());
		fin.seekg(0, std::ios::beg);
		//std::cout << "size is DATA IN GET BYTES: " << data.size() << " " << dirName << " " << fileName << std::endl;
		fin.read(&data[0], data.size());

		return data;
	}

	else
	{
		std::cout << "Failed to open " << dirName + "/" + fileName << ".\n";
		exit(1);
	}

}

// ./app.out input_file mutation_file crash_file example.out 2