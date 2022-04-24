#include "corpus-init.hpp"

#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif

CorpusInit::CorpusInit()
{

}

CorpusInit::CorpusInit(std::string dirForInputSamples, SampleProcessing sampleProcessing, int sizeOfCorpus)
{
    setDirForInputSamples(dirForInputSamples);
    setSampleProcessing(sampleProcessing);
    setSizeOfCorpus(sizeOfCorpus);
}

std::queue<Sample> CorpusInit::start()
{
    return createNew();
}

void CorpusInit::setDirForInputSamples(std::string dirForInputSamples)
{
    dirForInputSamples_ = dirForInputSamples;
}

void CorpusInit::setSampleProcessing(SampleProcessing sampleProcessing)
{
    sampleProcessing_ = sampleProcessing;
}

void CorpusInit::setSizeOfCorpus(int sizeOfCorpus)
{
    sizeOfCorpus_ = sizeOfCorpus;
}

std::string CorpusInit::dirForInputSamples()
{
    return dirForInputSamples_;
}

SampleProcessing CorpusInit::sampleProcessing()
{
    return sampleProcessing_;
}

int CorpusInit::sizeOfCorpus()
{
    return sizeOfCorpus_;
}

std::vector<std::string> CorpusInit::getListOfFiles()
{
    std::vector<std::string> result; 
    std::string path = dirForInputSamples();
    int counter = 0; 

    for (const auto & entry : fs::directory_iterator(path))
    {
        result.push_back(entry.path().filename());
        counter++;
    }

    return result;
}

void CorpusInit::copyInputFiles(int count, std::string data)
{
    for(int i = 0; i < count; i++)
    {
        sampleProcessing().createNew(data, "input_copy" + std::to_string(i + 1));
    }
}

std::queue<Sample> CorpusInit::createNew()
{
    std::queue<Sample> result;
    std::vector<std::string> fileNames = getListOfFiles();


    if(fileNames.size() < sizeOfCorpus())
    {
        copyInputFiles(sizeOfCorpus() - fileNames.size(), sampleProcessing().getBytes(fileNames.at(0)));
        fileNames = getListOfFiles();
    }

    for(int i = 0; i < fileNames.size(); i++)
    {
        CodeCoverage codeCoverage(fileNames.at(i));
        codeCoverage.start();
        Sample sample(sampleProcessing().getBytes(fileNames.at(i)), codeCoverage, fileNames.at(i));
        result.push(sample);
    }

    return result;
}