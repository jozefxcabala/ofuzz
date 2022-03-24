#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int fd;
    char myfifo [] = "myfifo";
    std::vector<uint64_t> data;
    int save_out;

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
            wait(NULL);
            switch(fork()){
                case -1:
                {
                    printf("fork nefunguje normalne");
                    return -1;
                }
                case 0:
                {
                    save_out = dup(STDOUT_FILENO);
                    if(save_out == -1)
                    {
                        fprintf(stderr,"Error in dup(STDOUT_FILENO)\n");
                        exit(EXIT_FAILURE);
                    }

                    int devNull = open("/dev/null",O_WRONLY);
                    if(devNull == -1)
                    {
                        fprintf(stderr,"Error in open('/dev/null',0)\n");
                        exit(EXIT_FAILURE);
                    }

                    int dup2Result = dup2(devNull, STDOUT_FILENO);
                    if(dup2Result == -1)
                    {
                        fprintf(stderr,"Error in dup2(devNull, STDOUT_FILENO)\n");
                        exit(EXIT_FAILURE);
                    }

                    execl("./a.out", "./a.out", (char*) NULL);
                }
                default:
                {
                    uint64_t address{0};
                    fd = open(myfifo, O_RDONLY);
                    while(read(fd, &address, sizeof(uint64_t))){
                        std::cout << address << std::endl;
                        data.push_back(address);
                    }
                    close(fd);   
                    unlink(myfifo);
                }
            }
        }
    }
    return 0;
}