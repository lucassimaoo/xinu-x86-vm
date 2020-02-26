#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

void func() {
    pid32 pid = getpid();
    kprintf("running PID:%d\n", pid);
    while(1) {}
}

shellcmd xsh_create(int nargs, char *args[])
{
    pri16 priority = 20;
    if (nargs >= 2) {
         priority = atoi(args[1]);
    }
    resume(create(func, 1024, priority, "LucasProcess", 0));
    return 0;
}