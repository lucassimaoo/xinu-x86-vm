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
    ptnum = ptcreate(maxConcurrent); // creates the port
}

// wrapper of function execution
void threadWraper(void (*procaddr)()) {
    pid32 pid = getpid();
    procaddr(); //executes the real function
    nthreads--; //after the function is done, decrements the thread counter
    kprintf("calling send %d\n", pid);
    ptsend(ptnum, pid); //sends a message to the port
}

// function to create the thread
void submit(void (*procaddr)()) {
    // notice that here we are using threadWraper function and passing the real function 
    // to be executed as a parameter to the threadWraper
    pid32 taskPid = create(threadWraper, 1024, 20, *tpname + "_" + nthreads, 1, procaddr);
    nthreads++; // increments the thread counter
    resume(taskPid);
}

// wait for all threads to be done
void waitForTasks() {
    // while there are threads executing, it will try to receive messages
    while (nthreads > 0) {
        ptrecv(ptnum); // blocking call if no message is available
        // this avoids busy waiting on the thread that calls waitForTasks
        // because it will only be called again once a message is sent
    }
}
/*
end of thread pool methods
*/

// sample function just to demonstrate processes waiting
void myfunc() {
    pid32 pid = getpid();
    kprintf("myfunc %d\n", pid);
    sleep(5);
    kprintf("woke up %d\n", pid);
}

shellcmd xsh_runthreads(int nargs, char *args[])
{
    // initialize thread pool with max number of threads set to 10
    initialize("my tp", 10);
    
    int numt = atoi(args[1]);

    // creates X threads based on shell command parameter
    int i = 0;
    for(; i < numt; i++) {
        submit(myfunc);
    }

    // waits for all threads to be done, this will block for as long as threads are running
    waitForTasks();
    kprintf("done waiting \n");

    return 0;
}