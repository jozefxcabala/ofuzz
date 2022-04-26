#include <string>
#include <vector>

#ifndef MUTATION_H
#define MUTATION_H

class Mutation 
{
    std::vector<std::string> magicVector_;
    std::string dirForMutations_;

    public:
        Mutation();
        Mutation(std::string dirForMutationsA);

        std::string start(int method, std::string data);
        
        std::vector<std::string> magicVector();
        std::string dirForMutations();

        void setMagicVector(std::vector<std::string> magicVector);
        void setDirForMutations(std::string dirForMutations);
    
    private:
        std::string bitFlip(std::string data);
        std::vector<std::string> vectorGen();
        std::string magic(std::string data, std::vector<std::string> magic);
        int pick();


};

#endif // MUTATION_H