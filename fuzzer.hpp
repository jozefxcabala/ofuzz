#include <string>
#include <queue>
#include "sample.hpp"

#ifndef FUZZER_H
#define FUZZER_H

class Fuzzer 
{
    std::queue<Sample> corups_;
    int numberOfIterations_;

    public:
        Fuzzer();
        Fuzzer(std::queue<Sample> corpusA, int numberOfIterationsA);

        std::queue<Sample> corpus();
        int numberOfIterations();
        
        void setCorpus(std::queue<Sample> corpus);
        void setNumberOfIterations(int numberOfIterations);

        void start();

};

#endif // FUZZER_H