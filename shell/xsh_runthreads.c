#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

/*
thread pool methods
*/
int32 nthreads;
char *tpname;

//TODO add max number of concurrent processes blocking with a semaphore
//TODO receive functions as parameters
//TODO parameters?

void initialize(char *name) {
    nthreads = 0;
    tpname = name;
}

void threadWraper(void (*procaddr)()) {
    pid32 pid = getpid();
    procaddr();
    nthreads--;
}

void submit(void (*procaddr)()) {
    nthreads++;
    pid32 taskPid = create(threadWraper, 1024, 20, *tpname + "_" + nthreads, 1, procaddr);
    resume(taskPid);
}

void waitForTasks() {
    while (nthreads > 0) {
        yield();
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
    initialize(20, "my tp");
    
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