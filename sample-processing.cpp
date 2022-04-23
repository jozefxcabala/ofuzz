#include "sample-processing.hpp"

#include <fstream>
#include <iostream>

void SampleProcessing::createNew(std::string mutated, std::string fileName)
{
	std::ofstream fout(fileName, std::ios::binary);

	if (fout.is_open())
	{
		fout.seekp(0, std::ios::beg);
		fout.write(&mutated[0], mutated.size());
	}
	else
	{
		std::cout << "Failed to create: " << fileName << std::endl; //TODO podla toho ako to mam inde
		exit(1);
	}

}

std::string SampleProcessing::getBytes(std::string fileName)
{
	std::ifstream fin(fileName, std::ios::binary);

	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		std::string data;
		data.resize(fin.tellg());
		fin.seekg(0, std::ios::beg);
		fin.read(&data[0], data.size());

		return data;
	}

	else
	{
		std::cout << "Failed to open " << fileName << ".\n";
		exit(1);
	}

}