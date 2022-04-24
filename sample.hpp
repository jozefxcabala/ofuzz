#include "code-coverage.hpp"

#include "string"

#ifndef SAMPLE_H
#define SAMPLE_H

class Sample 
{
    CodeCoverage codeCoverage_;
    std::string data_;
    std::string fileName_;

    public:
        Sample();
        Sample(std::string data, CodeCoverage codeCoverage, std::string fileName);

        std::string data();
        std::string fileName();
        CodeCoverage codeCoverage();

        void setData(std::string data);
        void setCodeCoverage(CodeCoverage codeCoverage);
        void setFileName(std::string fileName);
};

#endif // SAMPLE_H