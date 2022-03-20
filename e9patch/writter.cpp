#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    int fd;
    char myfifo [] = "myfifo";
    char buf[30];
    
    fd = open(myfifo, O_WRONLY);
    //char * instruction = (char*)malloc(sizeof(addr) * sizeof(char));
    //sprintf(instruction, "%p", addr);
    write(fd, "ahoj ty", sizeof(char) * 7);

    /* remove the FIFO */
    unlink(myfifo);
    
    return 0;
}