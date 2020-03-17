#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

pid32 p1;
pid32 p2;

void process1(char *name) {
    pid32 pid = getpid();
    int i = 0;
    for(; i < 10; i++) {
        kprintf("process %s, pid %d, prio %d\n", name, pid, getprio(pid));
        ready(p2, RESCHED_NO);
        suspend(p1);
    }
}

void process2(char *name) {
    pid32 pid = getpid();
    int i = 0;
    for(; i < 10; i++) {
        kprintf("process %s, pid %d, prio %d\n", name, pid, getprio(pid));     
        ready(p1, RESCHED_NO);
        suspend(p2);
    }
}

void pstarv() {
    pid32 pid = getpid();
    kprintf("process %s, pid %d, prio %d\n", "pstarv", pid, getprio(pid));
    starvpid = -1;
}

shellcmd xsh_runthreads(int nargs, char *args[])
{
    starvpid = create(pstarv, 1024, 25, "pstarv", 0);

    p1 = create(process1, 1024, 40, "p1", 1, "p1");
    p2 = create(process2, 1024, 35, "p2", 1, "p2");
    resume(p1);
    resume(p2);
    resume(starvpid);
    
    return 0;
}