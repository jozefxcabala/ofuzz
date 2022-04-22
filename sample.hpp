#include "code-coverage.hpp"

#include "string"

#ifndef SAMPLE_H
#define SAMPLE_H

class Sample 
{
    CodeCoverage codeCoverage_;
    std::string data_;

    public:
        Sample();

        std::string data();
        CodeCoverage codeCoverage();

        void setData(std::string data);
        void setCodeCoverage(CodeCoverage codeCoverage);
};

#endif // SAMPLE_H