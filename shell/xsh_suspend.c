#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

void funcSuspend() {
    pid32 pid = getpid();
    kprintf("running PID:%d\n", pid);
}

shellcmd xsh_suspend(int nargs, char *args[])
{
    pri16 priority = 20;
    if (nargs >= 2) {
         priority = atoi(args[1]);
    }
    create(funcSuspend, 1024, priority, "LucasProcess", 0);
    return 0;
}