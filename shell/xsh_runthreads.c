#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

/*
thread pool methods
*/
int32 nthreads;
char *tpname;
int32 ptnum;

void initialize(char *name, int32 maxConcurrent) {
    nthreads = 0;
    tpname = name;
    ptnum = ptcreate(maxConcurrent);
}

void threadWraper(void (*procaddr)()) {
    pid32 pid = getpid();
    procaddr();
    nthreads--;
    kprintf("calling send %d\n", pid);
    ptsend(ptnum, pid);
}

void submit(void (*procaddr)()) {
    pid32 taskPid = create(threadWraper, 1024, 20, *tpname + "_" + nthreads, 1, procaddr);
    nthreads++;
    resume(taskPid);
}

void waitForTasks() {
    while (nthreads > 0) {
        pid32 pid = ptrecv(ptnum);
        kprintf("received %d\n", pid);
    }
}
/*
end of thread pool methods
*/

void myfunc() {
    pid32 pid = getpid();
    kprintf("myfunc %d\n", pid);
    sleep(5);
    kprintf("woke up %d\n", pid);
}

shellcmd xsh_runthreads(int nargs, char *args[])
{
    initialize("my tp", 10);
    
    int numt = atoi(args[1]);

    int i = 0;
    for(; i < numt; i++) {
        submit(myfunc);
    }
    waitForTasks();
    kprintf("done waiting 1\n");

    i = 0;
    for(; i < numt; i++) {
        submit(myfunc);
    }

    waitForTasks();
    kprintf("done waiting 2\n");

    return 0;
}