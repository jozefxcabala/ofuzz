#include <string>
#include <vector>
#include <atomic>

#ifndef MUTATION_H
#define MUTATION_H

class Mutation 
{
    std::vector<std::string> magicVector_;
    std::string dirForMutations_;
    int id_;

    public:
        Mutation();
        Mutation(std::string dirForMutationsA, int id);

        std::string start(int method, std::string data, std::atomic<int>& MAGIC_NUMBERS, std::atomic<int>& BIT_FLIP);
        
        std::vector<std::string> magicVector();
        std::string dirForMutations();
        int id();

        void setMagicVector(std::vector<std::string> magicVector);
        void setDirForMutations(std::string dirForMutations);
        void setId(int id);
    
    private:
        std::string bitFlip(std::string data);
        std::vector<std::string> vectorGen();
        std::string magic(std::string data, std::vector<std::string> magic);
        int pick();


};

#endif // MUTATION_H