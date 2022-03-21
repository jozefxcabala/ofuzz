#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    int fd;
    char myfifo [] = "myfifo";
    char buf[10][12];

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
            execl("./e9patch/e9tool", "./e9patch/e9tool", "-M", "asm=/j.*/", "-P", "entry(addr)@instrumentation.out", "example.out", (char*) NULL);
        }
        default:
        {
            //parent

            fd = open(myfifo, O_RDONLY);
            read(fd, buf, sizeof(char) * 120);
            int length = sizeof(buf)/sizeof(buf[0][0]);
            for(int i = 0; i < length; i += 8){
                printf("recieved: %p length: %d\n", *(buf+i), length);
            }
            close(fd);   
        }
    }

    unlink(myfifo);
    return 0;
}