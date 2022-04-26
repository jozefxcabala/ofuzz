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

    CrashesProcessing crashesProcessing(argv[3]);
    SampleProcessing sampleProcessing;
    CorpusInit corpusInit(argv[1], sampleProcessing, 10);
    Mutation mutation;
    BinaryFileInstrumentation binaryFileInstrumentation(argv[4]);

    std::queue<Sample> corpus = corpusInit.start();
    binaryFileInstrumentation.start();

    Fuzzer fuzzer(sampleProcessing, mutation, crashesProcessing, corpus);

    fuzzer.start();

    //TODO spracovanie mena dir pre mutated files - asi to sprav tak aby si to priecinok vytvorilo samo 

    return 0;
}