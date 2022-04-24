#include "sample.hpp"

Sample::Sample()
{
}

Sample::Sample(std::string data, CodeCoverage codeCoverage, std::string fileName)
{
    setData(data);
    setCodeCoverage(codeCoverage);
    setFileName(fileName);
}

void Sample::setData(std::string data)
{
    data_ = data;
}

void Sample::setCodeCoverage(CodeCoverage codeCoverage)
{
    codeCoverage_ = codeCoverage;
}

void Sample::setFileName(std::string fileName)
{
    fileName_ = fileName;
}

std::string Sample::data()
{
    return data_;
}

std::string Sample::fileName()
{
    return fileName_;
}

CodeCoverage Sample::codeCoverage()
{
    return codeCoverage_;
}