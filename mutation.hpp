#include <string>
#include <vector>

#ifndef MUTATION_H
#define MUTATION_H

class Mutation 
{
    public:
        Mutation();

        void start();
    
    private:
        std::string bit_flip(std::string data);
        std::vector<std::string> vector_gen();
        std::string magic(std::string data, std::vector<std::string> magic);
        int pick();


};

#endif // MUTATION_H