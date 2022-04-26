#include "fuzzer.hpp"
#include "corpus-init.hpp"
#include "binary-file-instrumentation.hpp"
#include "iostream"

void checkParameters(int argc, char** argv)
{
    if (argc < 6)
	{
		std::cout << "Usage: ./app.out <dir for input samples> <dir for mutated files> <dir for crashes> <target application> <param>" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
    checkParameters(argc, argv);

    CorpusInit corpusInit(argv, 10);
    BinaryFileInstrumentation binaryFileInstrumentation(argv[4]);

    std::queue<Sample> corpus = corpusInit.start();
    binaryFileInstrumentation.start();

    Fuzzer fuzzer(corpus, 1);

    fuzzer.start();

    //TODO asi to sprav tak aby si to priecinok vytvorilo samo pri crash_file a mutated_file
    //TODO spracovanie argumentov pre spustenie programu

    return 0;
}