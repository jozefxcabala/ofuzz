all:
	g++ -std=c++17 app.cpp binary-file-instrumentation.cpp code-coverage.cpp sample.cpp mutation.cpp sample-processing.cpp corpus-init.cpp crashes-processing.cpp fuzzer.cpp -o app.out
	g++ -std=c++17 example.cpp -o example.out
	./e9patch/e9compile.sh instrumentation.c	