#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

pri16	resume_without_resched(
	  pid32		pid		/* ID of process to unsuspend	*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	procent *prptr;		/* ptr to process' table entry	*/
	pri16	prio;			/* priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16)SYSERR;
	}
	prptr = &proctab[pid];
	if (prptr->prstate != PR_SUSP) {
		restore(mask);
		return (pri16)SYSERR;
	}
	prio = prptr->prprio;		/* record priority to return	*/
	ready(pid, RESCHED_NO); // this is where we force no resched
	restore(mask);
	return prio;
}

shellcmd xsh_resumen(int nargs, char *args[])
{
    if (nargs >= 2) {
        int i;
        for (i = 1; i < nargs; i++) {
            // resuming each process
            resume_without_resched(atoi(args[i]));
        }
        //triggering resched after all of them are in the ready queue
        resched();
    }  
    return 0;
}