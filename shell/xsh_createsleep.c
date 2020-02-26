#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

void funcSleep() {
    sleepms(10000);
    pid32 pid = getpid();
    kprintf("running PID:%d\n", pid);
    while(1) {}
}

shellcmd xsh_createsleep(int nargs, char *args[])
{
    pri16 priority = 20;
    if (nargs >= 2) {
         priority = atoi(args[1]);
    }
    resume(create(funcSleep, 1024, priority, "LucasProcess", 0));
    return 0;
}