all:
	g++ fuzzer.cpp -o fuzzer.out
	g++ example.cpp -o example.out
	./e9patch/e9compile.sh instrumentation.c