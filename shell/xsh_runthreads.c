#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

sid32 semA;
sid32 semB;

void alice() {
    kprintf("A1 ");
    signal(semB);
    wait(semA);
    kprintf("A2 ");
}

void bob() {
    kprintf("B1 ");
    signal(semA);
    wait(semB);
    kprintf("B2 ");
}

shellcmd xsh_runthreads(int nargs, char *args[])
{

    semA = semcreate(0);
    semB = semcreate(0);

    int randomOrder = rand();
    int randomPriority = rand();

    pri16 priority = 20;

    // making second process higher priority
    if (randomPriority % 2 == 0) {
        priority = 21;
    }

    //alice thread goes first
    if (randomOrder % 2 == 0) {
        resume(create(alice, 1024, 20, "alice", 0));
        resume(create(bob, 1024, priority, "bob", 0));
    } else {
        resume(create(bob, 1024, 20, "bob", 0));
        resume(create(alice, 1024, priority, "alice", 0));
    }

    return 0;
}