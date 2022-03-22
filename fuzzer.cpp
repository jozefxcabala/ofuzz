#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>

int main(int argc, char* argv[])
{
    int fd;
    char myfifo [] = "myfifo";
    char buf[8]; // void* buf = ::operator new(sizeof(char) * 8);
    std::vector<std::string> data; //std::vector<void*> data;

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
            while(read(fd, buf, sizeof(char) * 8)){
                data.push_back(buf);
                //buf = ::operator new(sizeof(char) * 8);
            }

            for(std::string address: data){
                printf("j.* : %p\n", address);
            }

            //for(void* address: data){
            //    printf("j.* : %p\n", address);
            //}

            close(fd);   
        }
    }

    unlink(myfifo);
    return 0;
}