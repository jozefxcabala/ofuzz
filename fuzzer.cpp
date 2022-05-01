#include "fuzzer.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

Fuzzer::Fuzzer()
{

}

Fuzzer::Fuzzer(std::vector<Sample> corpusA, int numberOfIterationsA)
{   
    setCorpus(corpusA);
    setNumberOfIterations(numberOfIterationsA);
}

void Fuzzer::setCorpus(std::vector<Sample> corpus)
{
    corups_ = corpus;
}

std::vector<Sample>& Fuzzer::corpus()
{
    return corups_;
}

void Fuzzer::setNumberOfIterations(int numberOfIterations)
{
    numberOfIterations_ = numberOfIterations;
}

int Fuzzer::numberOfIterations()
{
    return numberOfIterations_;
}

static void handlerOfCtrlC(int s)
{
    printf("Caught signal: %d\n",s);
    KEEP_GOING.store(false);
}

void Fuzzer::fuzzing(int id)
{
    while(KEEP_GOING.load()){
        std::cout << "fuzzing by thread: " << id << std::endl;

        Sample sample = corpus().at(id);
        std::string previousData = sample.sampleProcessing().getBytes(sample.fileName(), sample.codeCoverage().argv()[2]); //TODO zmen to umiestnenie dirName // TU JE TIEZ CHYBA NACITA 0 BYTESinfo 
        int previousCoverage = sample.codeCoverage().coverage();

        std::string newData = sample.mutation().start(0, previousData);

        sample.sampleProcessing().createNew(newData, sample.fileName(), sample.codeCoverage().argv()[2]); //TODO zmen to umiestnenie dirName

        mutex.lock();
        sample.codeCoverage().start();
        mutex.unlock();

        int newCoverage = sample.codeCoverage().coverage();

        if(newCoverage > previousCoverage)
        {
            if(newCoverage > BEST_COVERAGE.load())
            {
                BEST_COVERAGE.store(newCoverage); 
                std::cout << "best coverage: " << BEST_COVERAGE.load() << " improved by thread: " << id << std::endl;
                //TODO distribucia najlepsej SAMPLE
            }
        }
        else
        {
            sample.sampleProcessing().createNew(previousData, sample.fileName(), sample.codeCoverage().argv()[2]); //TODO zmen to umiestnenie dirName
        }
    }
}

void Fuzzer::start()
{
    std::cout << "uspesne som spustil fuzzer!" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = handlerOfCtrlC;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    int counter = 0;
    BEST_COVERAGE.store(0);

    std::thread threads[corpus().size()];

    KEEP_GOING.store(true);

    for(int i = 0; i < corpus().size(); i++)
    {
        threads[i] = std::thread(&Fuzzer::fuzzing, this, std::ref(i));
    }

    for (int i = 0; i < corpus().size(); i++)
    {
        threads[i].join();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "execution time: " << duration.count() << "ms" << std::endl;
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


