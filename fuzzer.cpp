#include "fuzzer.hpp"
#include <iostream>
#include <chrono>

Fuzzer::Fuzzer()
{

}

Fuzzer::Fuzzer(std::queue<Sample> corpusA, int numberOfIterationsA)
{   
    setCorpus(corpusA);
    setNumberOfIterations(numberOfIterationsA);
}

void Fuzzer::setCorpus(std::queue<Sample> corpus)
{
    corups_ = corpus;
}

std::queue<Sample> Fuzzer::corpus()
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

void Fuzzer::start()
{
    std::cout << "uspesne som spustil fuzzer!" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    int counter = 0;

    while(counter < numberOfIterations())
    {
        while (!corpus().empty())
        {
            Sample corpusElement = corpus().front();
            corpusElement.codeCoverage().start();
            std::cout << corpusElement.fileName() << " - coverage: " << corpusElement.codeCoverage().coverage() << std::endl;
            corpusElement.codeCoverage().printData();
            corpus().pop();
            corpus().push(corpusElement);
            
            break;
        }

        counter++;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "execution time: " << duration.count() << "ms" << std::endl;
}