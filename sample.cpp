#include "sample.hpp"

Sample::Sample()
{
}

void Sample::setData(std::string data)
{
    data_ = data;
}

void Sample::setCodeCoverage(CodeCoverage codeCoverage)
{
    codeCoverage_ = codeCoverage;
}

std::string Sample::data()
{
    return data_;
}

CodeCoverage Sample::codeCoverage()
{
    return codeCoverage_;
}
