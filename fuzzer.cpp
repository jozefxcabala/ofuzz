#include "fuzzer.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include "logger.hpp"
#include "cstdio"
#include <unistd.h>

void menu(int size, std::string target, std::chrono::time_point<std::chrono::high_resolution_clock> start)
{
    auto end = std::chrono::high_resolution_clock::now();
    auto durationInSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double iteration = ITERATION.load();

    system("clear");

    if(iteration != 0 && durationInSeconds.count() != 0)
    {
        int day = durationInSeconds.count() / 86400.0;
        int hour = durationInSeconds.count() / 3600.0;
        int minutes = durationInSeconds.count() / 60.0;
        int seconds = durationInSeconds.count();

        printf(
        ".........................[ %03d days %02d hours %02d minutes %02d seconds ]........................\nIterations  : %d\nSpeed       : %d/sec\nCrashes     : %d\nCorpus Size : %d\nCoverage    : %d\nTarget      : %s\nBitFlip Mutation    : %dx\nMagic Mutation    : %dx\n............................................................................................\n",
        day % 365, hour % 24, minutes % 60, seconds % 60, ITERATION.load(), (int)(iteration / durationInSeconds.count()), CRASHES.load(), size, BEST_COVERAGE.load(), target.c_str(), BIT_FLIP.load(), MAGIC_NUMBERS.load());

    } else
    {
        printf(
        ".........................[ %03d days %02d hours %02d minutes %02d seconds ]........................\nIterations  : %d\nSpeed       : %d/sec\nCrashes     : %d\nCorpus Size : %d\nCoverage    : %d\nTarget      : %s\nBitFlip Mutation    : %dx\nMagic Mutation    : %dx\n............................................................................................\n",
        0, 0, 0, 0, ITERATION.load(), 0, CRASHES.load(), size, BEST_COVERAGE.load(), target.c_str(), BIT_FLIP.load(), MAGIC_NUMBERS.load());
    }
        

    
}

Fuzzer::Fuzzer()
{

}

Fuzzer::Fuzzer(std::vector<Sample> corpusA)
{   
    setCorpus(corpusA);
}

void Fuzzer::setCorpus(std::vector<Sample> corpus)
{
    corups_ = corpus;
}

std::vector<Sample>& Fuzzer::corpus()
{
    return corups_;
}

void Fuzzer::fuzzing(int id, std::string target, std::chrono::time_point<std::chrono::high_resolution_clock> start)
{
    bool firstIteration = true;
    LOG_INFO(id, "Thread %d was created and fuzzing by these thread was started", id);
    LOG_DEBUG(id, "Thread %d is trying access sample at corpus", id);
    mutex.lock();
    LOG_DEBUG(id, "Thread %d access sample at corpus", id);
    Sample sample = corpus().at(id);
    mutex.unlock();

    int counter = 0;

    while(true){
        menu(5, target, start);
        ITERATION.store(ITERATION.load() + 1);
        std::string previousData;
        LOG_DEBUG(id, "Thread %d is trying to get data with best coverage", id);
        mutex.lock();

        if(firstIteration || counter < 75)
        {
            previousData = sample.sampleProcessing().getBytes(sample.fileName(), sample.mutation().dirForMutations());
            firstIteration = false;
            counter++;
        } else
        {
            previousData = sample.sampleProcessing().getBytes("best-coverage", sample.mutation().dirForMutations());
            counter = 0;
        }
        
        mutex.unlock();
        LOG_DEBUG(id, "Thread %d got data with best coverage successfully", id);
  

        LOG_DEBUG(id, "Thread %d is trying to get previous coverage", id);
        int previousCoverage = sample.codeCoverage().coverage();
        LOG_DEBUG(id, "Thread %d previous coverage is: %d", id, previousCoverage);

        LOG_DEBUG(id, "Thread %d is trying to mutate data", id);
        std::string newData = sample.mutation().start(2, previousData, MAGIC_NUMBERS, BIT_FLIP);

        LOG_DEBUG(id, "Thread %d is trying to create rewrite data in orignal file", id);
        sample.sampleProcessing().createNew(newData, sample.fileName(), sample.mutation().dirForMutations());

        LOG_DEBUG(id, "Thread %d is trying to calculate new code coverage", id);
        mutex.lock();
        sample.codeCoverage().start();
        mutex.unlock();

        int newCoverage = sample.codeCoverage().coverage();
        LOG_DEBUG(id, "Thread %d new coverage is: %d", id, newCoverage);


        LOG_DEBUG(id, "Thread %d is checking if target application does not crashed", id); 
        mutex.lock();
        if(sample.crashesProcessing().start(newData))
        {
            CRASHES.store(CRASHES.load() + 1);
        }
        mutex.unlock();

        if(newCoverage > previousCoverage)
        {
            LOG_DEBUG(id, "Thread %d: new coverage %d is larger than previous coverage %d", id, newCoverage, previousCoverage);
            if(newCoverage > BEST_COVERAGE.load())
            {
                LOG_INFO(id, "Thread %d: we got new best coverage (%d), i am saving it and sending to others thread", id, newCoverage);

                LOG_DEBUG(id, "Thread %d start setting BEST_COVERAGE", id);
                mutex.lock();
                BEST_COVERAGE.store(newCoverage); 
                LOG_DEBUG(id, "Thread %d setting BEST_COVERAGE ended successfully", id);

                LOG_DEBUG(id, "Thread %d start updating best-coverage file", id);
                sample.sampleProcessing().createNew(newData, "best-coverage", sample.mutation().dirForMutations());
                mutex.unlock();
                LOG_DEBUG(id, "Thread %d updating best-coverage file ended successfully", id);
            }
        }
    }
}

void Fuzzer::start(std::string target)
{
    LOG_INFO(6, "Start of fuzzing");
    auto start = std::chrono::high_resolution_clock::now();

    LOG_DEBUG(6, "BEST_COVERAGE is setting to 0");
    BEST_COVERAGE.store(0);

    LOG_DEBUG(6, "ITERATION is setting to 0");
    ITERATION.store(0);

    LOG_DEBUG(6, "CRASHES is setting to 0");
    CRASHES.store(0);

    LOG_DEBUG(6, "BIT_FLIP is setting to 0");
    BIT_FLIP.store(0);

    LOG_DEBUG(6, "MAGIC_NUMBERS is setting to 0");
    MAGIC_NUMBERS.store(0);

    std::thread threads[corpus().size()];

    LOG_DEBUG(6, "Start of creating threads, count: %d", corpus().size());
    for(int i = 0; i < corpus().size(); i++)
    {
        LOG_DEBUG(6, "Thread %d is going to be create", i);
        threads[i] = std::thread(&Fuzzer::fuzzing, this, i, target, start);
    }

    LOG_DEBUG(6, "Start of joining threads");
    for (int i = 0; i < corpus().size(); i++)
    {
        LOG_DEBUG(6, "Thread %d is going to be joined", i);
        threads[i].std::thread::join();
        LOG_DEBUG(6, "Thread %d was joined", i);
    }

    LOG_INFO(6, "Fuzzing ended successfully");
}

