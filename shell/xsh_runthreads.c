#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

/*
thread pool methods
*/
sid32 threads;
pid32 callerPid;
//int16 threadtab[NPROC];
int32 nthreads = 0;

void initialize() {
    threads = semcreate(1);
}

void threadWraper(void (*procaddr)()) {
    pid32 pid = getpid();
    //kprintf("gonna call function %d\n", pid);
    procaddr();
    kprintf("calling wait %d\n", pid);
    wait(threads);
    kprintf("calling send %d\n", pid);
    //threadtab[pid] = 0;
    
    send(callerPid, pid);
}

void submit(void (*procaddr)()) {
    callerPid = getpid();
    pid32 taskPid = create(threadWraper, 1024, 20, "threadpool", 1, procaddr);
    //threadtab[taskPid] = 1;
    nthreads++;
    resume(taskPid);
}

void waitForTasks() {
    while (1) {
        
        //kprintf("receive %d\n", nthreads);
        pid32 pid = receive();
        kprintf("received %d\n", pid);
        signal(threads);
        nthreads--;
        kprintf("threads %d\n", nthreads);
        if (nthreads == 0) {
            break;
        }

       /* int32 i;
        for (i = 0; i < 100; i++) {
            if (threadtab[i] > 0) {
                kprintf("pid still running %d\n", i);
                goto receiveAnother;
            }
            
        }*/
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
    initialize();
    
    int numt = atoi(args[1]);

    int i = 0;
    for(; i < numt; i++) {
        submit(myfunc);
    }   
    waitForTasks();
    kprintf("done waiting\n");

    return 0;
}