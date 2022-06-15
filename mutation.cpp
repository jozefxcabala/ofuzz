#include "mutation.hpp"
#include <iostream>
#include "logger.hpp"
#include "time.h"

void Mutation::setId(int id)
{
	id_ = id;
}

int Mutation::id()
{
	return id_;
}

Mutation::Mutation()
{
}

Mutation::Mutation(std::string dirForMutationsA, int id)
{
	setDirForMutations(dirForMutationsA);
	setId(id);
}

std::vector<std::string> Mutation::magicVector()
{
    return magicVector_;
}

void Mutation::setMagicVector(std::vector<std::string> magicVector)
{
    magicVector_ = magicVector;
}

std::string Mutation::dirForMutations()
{
    return dirForMutations_;
}

void Mutation::setDirForMutations(std::string dirForMutations)
{
    dirForMutations_ = dirForMutations;
}

std::string Mutation::bitFlip(std::string data)
{
	LOG_INFO(id(), "Start of bitFlip method of mutation");
	int size = (data.length());
	int numOfFlips = (int)(size * .15);

	if(numOfFlips == 0)
	{
		numOfFlips = 1;
	}

	LOG_DEBUG(id(), "%d flips will be do", numOfFlips);

	// get a vector full of 1% of random byte indexes
	LOG_DEBUG(id(), "Start of picking indexes");
	std::vector<int> pickedIndexes;
	for (int i = 0; i < numOfFlips; i++)
	{
		int pickedIndex;
		if(size == 1)
		{
			pickedIndex = rand() % (size);
		}
		else
		{
			pickedIndex = rand() % (size - 1);
		}
		
		pickedIndexes.push_back(pickedIndex);
	}

	LOG_DEBUG(id(), "indexes was picked successfully");

	// iterate through the data string at those indexes and flip a bit
	LOG_DEBUG(id(), "Star of bit flipping");
	for (int i = 0; i < pickedIndexes.size(); ++i)
	{
		int index = pickedIndexes[i];
		char current = data.at(index);
		int decimal = ((int)current & 0xff);
		
		int bitToFlip = rand() % 8;
		
		decimal ^= 1 << bitToFlip;
		decimal &= 0xff;
		
		data[index] = (char)decimal;
	}
	LOG_DEBUG(id(), "Bit flipping ended successfully");

	LOG_INFO(id(), "BitFlip method of mutation ended successfully");

	return data;

}

std::vector<std::string> Mutation::vectorGen()
{
	LOG_INFO(id(), "Start of generating vector of magic numbers");
	std::vector<std::string> magic;

	using namespace std::string_literals;

	magic.push_back("\xff");
	magic.push_back("\x7f");
	magic.push_back("\x00"s);
	magic.push_back("\xff\xff");
	magic.push_back("\x7f\xff");
	magic.push_back("\x00\x00"s);
	magic.push_back("\xff\xff\xff\xff");
	magic.push_back("\x80\x00\x00\x00"s);
	magic.push_back("\x40\x00\x00\x00"s);
	magic.push_back("\x7f\xff\xff\xff");

	LOG_INFO(id(), "Vector of magic numbers was generated successfully");

	return magic;
}

std::string Mutation::magic(std::string data, std::vector<std::string> magic)
{
	LOG_INFO(id(), "Start of magic method of bytes mutation");
	int vectorSize = magic.size();
	LOG_DEBUG(id(), "Size of vector with magic values: %d", vectorSize);
	int pickedMagicIndex = rand() % vectorSize;
	LOG_DEBUG(id(), "Picked magic index: %d", pickedMagicIndex);
	std::string pickedMagic = magic[pickedMagicIndex];
	LOG_DEBUG(id(), "picked magic value: %s", pickedMagic.c_str());
	int size = (data.length());
	LOG_DEBUG(id(), "Size of data: %d", size);
	int pickedDataIndex = rand() % size;
	data.replace(pickedDataIndex, magic[pickedMagicIndex].length(), magic[pickedMagicIndex]);

	LOG_INFO(id(), "Magic method of bytes mutation ended successfully");

	return data;
}

int Mutation::pick()
{
	LOG_INFO(id(), "Start of pick mutation method");
	int result = rand() % 2;
	LOG_INFO(id(), "%d method was picked successfully", result);

	return result;
}

std::string Mutation::start(int method, std::string data, std::atomic<int>& MAGIC_NUMBERS, std::atomic<int>& BIT_FLIP)
{
	srand ( time(NULL) );
	LOG_INFO(id(), "Start of mutation");
    std::string mutated;
    int function;

    if (magicVector().size() == 0)
    {
        setMagicVector(vectorGen());
    }


    if (method == 0 || method == 1)
    {
        function = method;
    }
    else
    {
        function = pick();
    }
    

    if (function == 0)
    {
        // utilize the magic mutation method;
        mutated = magic(data, magicVector());
		MAGIC_NUMBERS.store(MAGIC_NUMBERS.load() + 1);
    }
    else
    {
        // utilize the bit flip mutation;
        mutated = bitFlip(data);
		BIT_FLIP.store(BIT_FLIP.load() + 1);
    }

	LOG_INFO(id(), "Mutation ended successfully");

    return mutated;
}