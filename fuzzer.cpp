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
    std::vector<uint64_t> data;

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

            uint64_t address{0};
            fd = open(myfifo, O_RDONLY);
            while(read(fd, &address, sizeof(uint64_t))){
                std::cout << address << std::endl;
                data.push_back(address);
            }

            close(fd);   
        }
    }

    unlink(myfifo);
    return 0;
}