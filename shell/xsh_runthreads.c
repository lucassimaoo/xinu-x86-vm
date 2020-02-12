#include <xinu.h>
#include <stdio.h>

/*
thread pool methods
*/
void submit(void * procaddr) {
    resume(create(procaddr, 1024, 20, "runloop", 0));
}

void myfunc() {
    kprintf("its on myfunc");
}

shellcmd xsh_runthreads(int nargs, char *args[])
{

    submit(myfunc);
    return 0;
}