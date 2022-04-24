#include "fuzzer.hpp"
#include "corpus-init.hpp"
#include "iostream"

void checkParameters(int argc, char** argv)
{
    if (argc < 5)
	{
		std::cout << "Usage: ./app.out <target application> <dir for input samples> <dir for mutated files> <dir for crashes>" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
    checkParameters(argc, argv);

    BinaryFileInstrumentation binaryFileInstrumentation(argv[1]);
    SampleProcessing sampleProcessing(argv[3]);
    Mutation mutation;
    CrashesProcessing crashesProcessing(argv[4]);
    CorpusInit corpusInit(argv[2], sampleProcessing, 10);
    std::queue<Sample> corpus = corpusInit.start();
    Fuzzer fuzzer(binaryFileInstrumentation, sampleProcessing, mutation, crashesProcessing, corpus);
    //fuzzer.start()

    return 0;
}