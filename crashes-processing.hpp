#include <string>

#ifndef CRASHES_PROCESSING_H
#define CRASHES_PROCESSING_H

class CrashesProcessing 
{
    std::string difForCrashFiles_;

    public:
        CrashesProcessing();
        CrashesProcessing(std::string dirForCrashFiles);

        void createNew(std::string crash, std::string fileName);

        void setDirForCrashFiles(std::string dirForCrashFiles);
        std::string dirForCrashFiles();
};

#endif // CRASHES_PROCESSING_H