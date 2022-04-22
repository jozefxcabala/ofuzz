all:
	g++ app.cpp binary-file-instrumentation.cpp code-coverage.cpp -o app.out
	g++ example.cpp -o example.out
	./e9patch/e9compile.sh instrumentation.c	