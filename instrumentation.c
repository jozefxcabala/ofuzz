#include "e9patch/examples/stdlib.c"

/*
 * The counter.
 */
static int fd;
char myfifo [] = "myfifo";

void entry(const void * addr)
{   
    write(fd, addr, sizeof(char) * 8);
}

/*
 * Initialization.
 */
void init(int argc, char **argv, char **envp)
{
    /* write to the FIFO */
    fd = open(myfifo, O_WRONLY);
}
