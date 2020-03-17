/* chprio.c - chprio */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  chprio  -  Change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
pri16	chprio(
	  pid32		pid,		/* ID of process to change	*/
	  pri16		newprio		/* new priority			*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	procent *prptr;		/* ptr to process' table entry	*/
	pri16	oldprio;		/* priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16) SYSERR;
	}
	prptr = &proctab[pid];
	oldprio = prptr->prprio;
	prptr->prprio = newprio;

	switch (prptr->prstate) {
	case PR_READY:
		//if ready, reinsert it to the ready list with the new priority
		insert( getitem(pid), readylist, newprio);
		break;
	case PR_CURR:
		//if current do nothing
		break;
	default:
		//if any other state, change it to ready
		ready(pid, RESCHED_NO);
		break;
	}
	
	restore(mask);
	return oldprio;
}
