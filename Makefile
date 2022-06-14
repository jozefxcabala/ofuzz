all:
	g++ -std=c++17 -g app.cpp binary-file-instrumentation.cpp code-coverage.cpp sample.cpp mutation.cpp sample-processing.cpp corpus-init.cpp crashes-processing.cpp fuzzer.cpp -o app.out -lpthread
	./e9patch/e9compile.sh instrumentation.c	