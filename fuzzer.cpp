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
    bool firstIteration = true;
    std::string suffix = corpus().at(id).fileName().substr(corpus().at(id).fileName().find("."));
    LOG_INFO(id, "Thread %d was created and fuzzing by these thread was started", id);
    LOG_DEBUG(id, "Thread %d is trying access sample at corpus", id);
    mutex.lock();
    LOG_DEBUG(id, "Thread %d access sample at corpus", id);
    Sample sample = corpus().at(id);
    mutex.unlock();

    while(true){
        //todo create some GUI, tu sa bude volat nejaka funkcia, ktora bude zobrazovat data o aplikacii + dole sa bude vypisovat ak vznikne nejaky crash
        system("clear");
        ITERATION.store(ITERATION.load() + 1);
        LOG_APP(6, "Iteration: %d", ITERATION.load());
        std::string previousData;
        LOG_DEBUG(id, "Thread %d is trying to get data with best coverage", id);
        mutex.lock();

        if(firstIteration)
        {
            previousData = sample.sampleProcessing().getBytes(sample.fileName(), sample.mutation().dirForMutations());
            firstIteration = false;
        } else
        {
            previousData = sample.sampleProcessing().getBytes("best-coverage", sample.mutation().dirForMutations());
        }
        
        mutex.unlock();
        LOG_DEBUG(id, "Thread %d got data with best coverage successfully", id);
  

        LOG_DEBUG(id, "Thread %d is trying to get previous coverage", id);
        int previousCoverage = sample.codeCoverage().coverage();
        LOG_DEBUG(id, "Thread %d previous coverage is: %d", id, previousCoverage);

        LOG_DEBUG(id, "Thread %d is trying to mutate data", id);
        std::string newData = sample.mutation().start(0, previousData);

        LOG_DEBUG(id, "Thread %d is trying to create rewrite data in orignal file", id);
        sample.sampleProcessing().createNew(newData, sample.fileName(), sample.mutation().dirForMutations()); //TODO dopln thread do debugov a filenames

        LOG_DEBUG(id, "Thread %d is trying to calculate new code coverage", id);
        mutex.lock();
        sample.codeCoverage().start();
        mutex.unlock();

        int newCoverage = sample.codeCoverage().coverage();
        LOG_DEBUG(id, "Thread %d new coverage is: %d", id, newCoverage);


        LOG_DEBUG(id, "Thread %d is checking if target application does not crashed", id); 
        mutex.lock();
        sample.crashesProcessing().start(newData); //TODO musi byt coverage lepsi ked sposobi crash alebo natom nezalezi 
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

void Fuzzer::start()
{
    LOG_INFO(6, "Start of fuzzing");
    auto start = std::chrono::high_resolution_clock::now();

    LOG_DEBUG(6, "BEST_COVERAGE is setting to 0");
    BEST_COVERAGE.store(0);

    LOG_DEBUG(6, "ITERATION is setting to 0");
    ITERATION.store(0);

    std::thread threads[corpus().size()];

    LOG_DEBUG(6, "Start of creating threads, count: %d", corpus().size());
    for(int i = 0; i < corpus().size(); i++)
    {
        LOG_DEBUG(6, "Thread %d is going to be create", i);
        threads[i] = std::thread(&Fuzzer::fuzzing, this, i);
    }

    LOG_DEBUG(6, "Start of joining threads");
    for (int i = 0; i < corpus().size(); i++)
    {
        LOG_DEBUG(6, "Thread %d is going to be joined", i);
        threads[i].std::thread::join();
        LOG_DEBUG(6, "Thread %d was joined", i);
    }

    LOG_INFO(6, "Fuzzing ended successfully");

    // TODO toto sprav v crash handling
    // TODO krajsi vypis v konzole (farbicky pridaj pre kazdu thread)


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


