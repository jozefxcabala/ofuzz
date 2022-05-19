#include <string>

#ifndef CRASHES_PROCESSING_H
#define CRASHES_PROCESSING_H

class CrashesProcessing 
{
    std::string difForCrashFiles_;
    std::string inputFile_;
    char** argv_;
    int argc_;

    public:
        CrashesProcessing();
        CrashesProcessing(std::string dirForCrashFiles, char** argv, int argc, std::string inputFile);

        void createNew(std::string data, std::string fileName);

        void setInputFile(std::string inputFile);
        void setArgv(char** argv);
        void setArgc(int argc);
        void setDirForCrashFiles(std::string dirForCrashFiles);
        
        std::string inputFile();
        char** argv();
        int argc();
        std::string dirForCrashFiles();

        void start(std::string data);

    private:
        std::string getOutput();
        void checkForCrash(std::string output, std::string data);
        std::string getDate();
};

#endif // CRASHES_PROCESSING_H