#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

int32 ptnum;
int maxMessages;

void produce() {

    int i = 0;
    // producing 3 more messages to showcase the limit
    for(; i < maxMessages + 3; i++) {
        ptsend(ptnum, i);
        kprintf("sent %d\n", i);
    }
}

void consume() {
    sleepms(10000);

    while(1) {
        uint32 msg = ptrecv(ptnum);
        kprintf("received %d\n", msg);
    }
}

shellcmd xsh_runthreads(int nargs, char *args[])
{
    maxMessages = atoi(args[1]);
    ptnum = ptcreate(maxMessages);

    resume(create(produce, 1024, 20, "produce", 10));
    resume(create(consume, 1024, 20, "consume", 10));

    return 0;
}