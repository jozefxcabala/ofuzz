#include "fuzzer.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include "logger.hpp"

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

void Fuzzer::fuzzing(int id)
{
    LOG_INFO("Thread %d was created and fuzzing by these thread was started", id);
    while(true){
        LOG_DEBUG("Thread %d is trying access sample at corpus", id);
        mutex.lock();
        LOG_DEBUG("Thread %d access sample at corpus", id);
        Sample sample = corpus().at(id);
        mutex.unlock();

        LOG_DEBUG("Thread %d is trying to get previous data", id);
        std::string previousData = sample.sampleProcessing().getBytes(sample.fileName(), sample.mutation().dirForMutations()); //TODO zmen to umiestnenie dirName // TU JE TIEZ CHYBA NACITA 0 BYTESinfo 
        LOG_DEBUG("Thread %d is trying to get previous coverage", id);
        int previousCoverage = sample.codeCoverage().coverage();
        LOG_DEBUG("Thread %d previous coverage is: %d", id, previousCoverage);

        LOG_DEBUG("Thread %d is trying to mutate data", id);
        std::string newData = sample.mutation().start(0, previousData);

        LOG_DEBUG("Thread %d is trying to create rewrite data in orignal file", id);
        sample.sampleProcessing().createNew(newData, "thread-" + std::to_string(id) + "-" + sample.fileName(), sample.mutation().dirForMutations()); //TODO dopln thread do debugov a filenames

        LOG_DEBUG("Thread %d is trying to calculate new code coverage", id);
        mutex.lock();
        sample.codeCoverage().start();
        mutex.unlock();

        int newCoverage = sample.codeCoverage().coverage();
        LOG_DEBUG("Thread %d new coverage is: %d", id, newCoverage);

        if(newCoverage > previousCoverage)
        {
            LOG_DEBUG("Thread %d: new coverage %d is larger than previous coverage %d", id, newCoverage, previousCoverage);
            if(newCoverage > BEST_COVERAGE.load())
            {
                LOG_INFO("Thread %d: we got new best coverage (%d), i am saving it and sending to others thread (TO DO)", id, newCoverage);
                BEST_COVERAGE.store(newCoverage); 
                //TODO distribucia najlepsej SAMPLE
            }
        }
        else
        {
            LOG_DEBUG("Thread %d: new coverage %d is lower than previous coverage %d, rewrite file back to original state", id, newCoverage, previousCoverage);
            sample.sampleProcessing().createNew(previousData, "thread-" + std::to_string(id) + "-" + sample.fileName(), sample.codeCoverage().argv()[2]); //TODO zmen to umiestnenie dirName
        }
    }
}

void Fuzzer::start()
{
    LOG_INFO("Start of fuzzing");
    auto start = std::chrono::high_resolution_clock::now();

    int counter = 0;
    LOG_DEBUG("BEST_COVERAGE is setting to 0");
    BEST_COVERAGE.store(0);

    std::thread threads[corpus().size()];

    LOG_DEBUG("Start of creating threads, count: %d", corpus().size());
    for(int i = 0; i < corpus().size(); i++)
    {
        LOG_DEBUG("Thread %d is going to be create", i);
        threads[i] = std::thread(&Fuzzer::fuzzing, this, i);
    }

    LOG_DEBUG("Start of joining threads");
    for (int i = 0; i < corpus().size(); i++)
    {
        LOG_DEBUG("Thread %d is going to be joined", i);
        threads[i].std::thread::join();
        LOG_DEBUG("Thread %d was joined", i);
    }

    LOG_INFO("Fuzzing ended successfully");

    // TODO toto sprav v crash handling
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    // std::cout << "execution time: " << duration.count() << "ms" << std::endl;
}

/***
 * 1. spustenie
 *  vypocitaj code coverage
 * 2. - posledne spusteni
 *  nacitanie dat
 *  mutacia
 *  code coverage
 *  vyhodnotenie, ci je code coverage lepsi ako ten predtym v danej sample
 *      ak je lepsi, nechaj
 *      ak nie je lepsi, zahod mutaciu
 *  vyhodnoteniel ci je code coverage najlepsi celkovo
 *      ak je zamkni vsetko a odomknutu nechaj len thread, ktora ma najlepsi code coverage, ten uloz a rozposli ostatnym
 *      ak nie je nerob nic
 *  ak nastane pad programu, ulozh crash
 * 
 * mal by si mat samlples ulozene v niecom inom ako queue
 ***/


