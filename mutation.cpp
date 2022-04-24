#include "mutation.hpp"

Mutation::Mutation()
{
	
}

std::vector<std::string> Mutation::magicVector()
{
    return magicVector_;
}

void Mutation::setMagicVector(std::vector<std::string> magicVector)
{
    magicVector_ = magicVector;
}

std::string Mutation::bitFlip(std::string data)
{
	
	int size = (data.length() - 4);
	int numOfFlips = (int)(size * .01);

	// get a vector full of 1% of random byte indexes
	std::vector<int> pickedIndexes;
	for (int i = 0; i < numOfFlips; i++)
	{
		int pickedIndex = rand() % size;
		pickedIndexes.push_back(pickedIndex);
	}

	// iterate through the data string at those indexes and flip a bit
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

	return data;

}

std::vector<std::string> Mutation::vectorGen()
{
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

	return magic;
}

std::string Mutation::magic(std::string data, std::vector<std::string> magic)
{
	
	int vectorSize = magic.size();
	int pickedMagicIndex = rand() % vectorSize;
	std::string pickedMagic = magic[pickedMagicIndex];
	int size = (data.length() - 4);
	int pickedDataIndex = rand() % size;
	data.replace(pickedDataIndex, magic[pickedMagicIndex].length(), magic[pickedMagicIndex]);

	return data;

}

int Mutation::pick()
{
	int result = rand() % 2;

	return result;
}

std::string Mutation::start(int method, std::string data)
{
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
    }
    else
    {
        // utilize the bit flip mutation;
        mutated = bitFlip(data);
    }

    return mutated;
}