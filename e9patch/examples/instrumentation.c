#include "stdlib.c"


void entry(const void * addr)
{
    printf("jmp: %p\n", addr);
}

//./e9tool -M 'asm=/j.*/' -P 'entry(addr)@instrumentation' examples/example
