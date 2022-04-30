#include <iostream>

int main(int argc, char** argv){
    int x = 3;
    int y = 4;

    if(argc < 2){
        std::cout << "Zabudol si zadat argument" << std::endl;
        return -1;
    }

    for(int i = 0; i < 3; i++)
    {
        if((x + y) < 8)
        {
            std::cout << i << std::endl;
        } else {
            std::cout << -i << std::endl;
        }
    }

    return 0;
}   