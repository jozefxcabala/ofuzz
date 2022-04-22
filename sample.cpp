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

void Sample::data()
{
    return data_;
}

void Sample::codeCoverage()
{
    return codeCoverage_;
}
