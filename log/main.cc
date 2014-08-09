#include <iostream>
#include "log_leveldb.h"

void *log(void *)
{
    Logger *logger;
    NewLogger("log_test.log", &logger);

    Log(logger, "------------test");

    delete logger;
}


int main()
{
    pthread_t pid;
    pthread_create(&pid, NULL, log, NULL);
    pthread_join(pid, NULL);

    //printf("%x\n", 

    char a[24];
    sprintf(a, "%x", pid);
    std::cout << a << "end" << std::endl;
    return 0;
}
