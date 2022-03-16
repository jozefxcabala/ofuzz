#include <iostream>

int main(int argc, char* argv[])
{
    const char * instrumentation = "cd ~/bakalarka_kod/e9patch; ./e9tool -M 'asm=/j.*/' -P 'entry(addr)@instrumentation' examples/example";
    system(instrumentation);
    
    return 0;
}