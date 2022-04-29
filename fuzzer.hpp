#include <string>
#include <vector>
#include <atomic>
#include "sample.hpp"

#ifndef FUZZER_H
#define FUZZER_H

static std::atomic<bool> KEEP_GOING;
static std::atomic<int> BEST_COVERAGE;


class Fuzzer 
{
    std::vector<Sample> corups_;
    int numberOfIterations_;

    public:
        Fuzzer();
        Fuzzer(std::vector<Sample> corpusA, int numberOfIterationsA);

        std::vector<Sample>& corpus();
        int numberOfIterations();
        
        void setCorpus(std::vector<Sample> corpus);
        void setNumberOfIterations(int numberOfIterations);

        void start();

    private:
        void fuzzing(int id);

};

#endif // FUZZER_H