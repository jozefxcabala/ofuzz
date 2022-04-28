#include "code-coverage.hpp"
#include "crashes-processing.hpp"
#include "mutation.hpp"
#include "sample-processing.hpp"

#include "string"

#ifndef SAMPLE_H
#define SAMPLE_H

class Sample 
{
    std::string fileName_;
    CodeCoverage codeCoverage_;
    SampleProcessing sampleProcessing_;
    CrashesProcessing crashesProcessing_;
    Mutation mutation_;

    public:
        Sample();
        Sample(SampleProcessing sampleProcessing, CodeCoverage codeCoverage, std::string fileName, CrashesProcessing crashesProcessing, Mutation mutation);

        std::string fileName();
        CodeCoverage& codeCoverage();
        SampleProcessing sampleProcessing();
        CrashesProcessing crashesProcessing();
        Mutation mutation();

        void setFileName(std::string fileName);
        void setCodeCoverage(CodeCoverage codeCoverage);
        void setSampleProcessing(SampleProcessing sampleProcessing);
        void setCrashesProcessing(CrashesProcessing crashesProcessing);
        void setMutation(Mutation mutation);
        
};

#endif // SAMPLE_H