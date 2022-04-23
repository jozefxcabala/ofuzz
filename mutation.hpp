#include <string>
#include <vector>

#ifndef MUTATION_H
#define MUTATION_H

class Mutation 
{
    std::vector<std::string> magicVector_;

    public:
        Mutation();

        std::string start(int method, std::string data);
        
        std::vector<std::string> magicVector();
        void setMagicVector(std::vector<std::string> magicVector);
    
    private:
        std::string bitFlip(std::string data);
        std::vector<std::string> vectorGen();
        std::string magic(std::string data, std::vector<std::string> magic);
        int pick();


};

#endif // MUTATION_H