#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <signal.h>
#include "sample.hpp"

#ifndef FUZZER_H
#define FUZZER_H

static std::atomic<int> BEST_COVERAGE;
static std::mutex mutex;

class Fuzzer 
{
    std::vector<Sample> corups_;

    public:
        Fuzzer();
        Fuzzer(std::vector<Sample> corpusA);

        std::vector<Sample>& corpus();

        void setCorpus(std::vector<Sample> corpus);

        void start();

    private:
        void fuzzing(int id);

};

#endif // FUZZER_H