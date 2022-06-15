#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <signal.h>
#include "sample.hpp"
#include <string>
#include <chrono>

#ifndef FUZZER_H
#define FUZZER_H

static std::atomic<int> BEST_COVERAGE;
static std::atomic<int> ITERATION;
static std::atomic<int> CRASHES;
static std::atomic<int> BIT_FLIP;
static std::atomic<int> MAGIC_NUMBERS;
static std::mutex mutex;

class Fuzzer 
{
    std::vector<Sample> corups_;

    public:
        Fuzzer();
        Fuzzer(std::vector<Sample> corpusA);

        std::vector<Sample>& corpus();

        void setCorpus(std::vector<Sample> corpus);

        void start(std::string target);

    private:
        void fuzzing(int id, std::string target, std::chrono::time_point<std::chrono::high_resolution_clock> start);

};

#endif // FUZZER_H