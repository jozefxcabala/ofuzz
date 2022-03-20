#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    int fd;
    char myfifo [] = "myfifo";
    char buf[1000];

    if(mkfifo(myfifo, 0777) == -1){
        printf("pipe sa neotvorila");
        return -1;
    }
    
    switch(fork()){
        case -1:
        {
            printf("fork nefunguje normalne");
            return -1;
        }
        case 0:
        {
            //child
            execl("./e9tool", "./e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation", "examples/example", (char*) NULL);
        }
        default:
        {
            //parent
            fd = open(myfifo, O_RDONLY);
            read(fd, buf, sizeof(char) * 1000);
            printf("Received: %p\n", buf);
            close(fd);
        }
    }

    unlink(myfifo);
    return 0;
}