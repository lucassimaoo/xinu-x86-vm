#include <xinu.h>
#include <stdio.h>

void funcWait() {
    pid32 pid = getpid();
    kprintf("waiting PID:%d\n", pid);
    wait(midTermSem);
}

shellcmd xsh_wait(int nargs, char *args[])
{
    resume(create(funcWait, 1024, 20, "LucasProcess", 0));
    return 0;
}