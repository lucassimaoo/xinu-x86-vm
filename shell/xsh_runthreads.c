#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

/*
thread pool methods
*/
sid32 semthread;
pid32 callerPid;
int32 nthreads;
char *tpname;

void initialize(char *name) {
    nthreads = 0;
    semthread = semcreate(1);
    tpname = name;
}

void threadWraper(void (*procaddr)()) {
    pid32 pid = getpid();
    procaddr();
    wait(semthread);
    nthreads--;
    kprintf("calling send %d\n", pid);
    send(callerPid, pid);
}

void submit(void (*procaddr)()) {
    callerPid = getpid();
    pid32 taskPid = create(threadWraper, 1024, 20, *tpname + "_" + nthreads, 1, procaddr);
    nthreads++;
    resume(taskPid);
}

void waitForTasks() {
    while (nthreads > 0) {
        pid32 pid = receive();
        signal(semthread);
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
    initialize("my tp");
    
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