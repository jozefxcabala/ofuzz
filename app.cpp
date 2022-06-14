#include "fuzzer.hpp"
#include "corpus-init.hpp"
#include "binary-file-instrumentation.hpp"
#include "iostream"
#include "logger.hpp"
#include "assert.h"
#include <sys/stat.h>

void checkParameters(int argc, char** argv)
{
    LOG_INFO(6, "Checking parameters");
    if (argc < 3)
	{
        LOG_ERROR(6, "Usage: ./app.out <dir for input samples> <target application> <param>");
		exit(EXIT_FAILURE);
	}
    LOG_INFO(6, "Parameters are ok");
}

int main(int argc, char** argv)
{
    
    LOG_INFO(6, "App was started!");
    LOG_DEBUG(6, "Removing of old myfifo");

    struct stat info;

    if(stat("myfifo", &info) == 0)
    {
        int status = system("rm myfifo");
        
        if(status != 0)
        {
            LOG_ERROR(6, "Removing of old myfifo ended unsuccessfully");
            exit(EXIT_FAILURE);
        }

        LOG_DEBUG(6, "Removing of old myfifo ended successfully");
    } else
    {
        LOG_DEBUG(6, "There is no file myfifo");
    }

    

    checkParameters(argc, argv);

    CorpusInit corpusInit(argv, 39, argc);
    BinaryFileInstrumentation binaryFileInstrumentation(argv[2]);

    std::vector<Sample> corpus = corpusInit.start();
    binaryFileInstrumentation.start();

    Fuzzer fuzzer(corpus);

    fuzzer.start();

    //TODO asi to sprav tak aby si to priecinok vytvorilo samo pri crash_file a mutated_file

    return 0;
}