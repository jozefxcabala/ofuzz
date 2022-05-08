#include "fuzzer.hpp"
#include "corpus-init.hpp"
#include "binary-file-instrumentation.hpp"
#include "iostream"
#include "logger.hpp"
#include "assert.h"

void checkParameters(int argc, char** argv)
{
    if (argc < 3)
	{
		std::cout << "Usage: ./app.out <dir for input samples> <target application> <param>" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
    LOG_INFO("App was started!");
    checkParameters(argc, argv);

    CorpusInit corpusInit(argv, 10, argc);
    BinaryFileInstrumentation binaryFileInstrumentation(argv[2]);

    std::vector<Sample> corpus = corpusInit.start();
    binaryFileInstrumentation.start();

    Fuzzer fuzzer(corpus);

    fuzzer.start();

    //TODO asi to sprav tak aby si to priecinok vytvorilo samo pri crash_file a mutated_file

    return 0;
}