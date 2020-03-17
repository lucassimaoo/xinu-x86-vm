#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

void processFunction(char *name) {
    pid32 pid = getpid();
    int i = 0;
    for(; i < 10; i++) {
        kprintf("process %s, pid %d, prio %d\n", name, pid, getprio(pid));
        //yield();
        sleepms(2);
    }
}

void pstarv() {
    pid32 pid = getpid();
    kprintf("process %s, pid %d, prio %d\n", "pstarv", pid, getprio(pid));
}


shellcmd xsh_runthreads(int nargs, char *args[])
{
    //int sleeptime = atoi(args[1]);
    starvpid = create(pstarv, 1024, 25, "pstarv", 0);
    //kprintf("init starvpid %d,\n", starvpid);

    resume(create(processFunction, 1024, 40, "p1", 2, "p1"));
    resume(create(processFunction, 1024, 35, "p2", 2, "p2"));
    resume(create(processFunction, 1024, 30, "p3", 2, "p3"));
    resume(create(processFunction, 1024, 45, "p4", 2, "p4"));
    resume(create(processFunction, 1024, 40, "p5", 2, "p5"));
    resume(create(processFunction, 1024, 35, "p6", 2, "p6"));
    resume(create(processFunction, 1024, 30, "p7", 2, "p7"));
    resume(create(processFunction, 1024, 45, "p8", 2, "p8"));
    

    resume(starvpid);
    
    return 0;
}