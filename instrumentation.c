#include "e9patch/examples/stdlib.c"

/*
 * The counter.
 */
static int fd;
char myfifo [] = "myfifo";

void entry(const void * addr)
{   
    write(fd, (uint64_t)&addr, sizeof(uint64_t));
}

/*
 * Initialization.
 */
void init(int argc, char **argv, char **envp)
{
    /* write to the FIFO */
    fd = open(myfifo, O_WRONLY);
}
