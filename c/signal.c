/* signal.c - support for signal handling
   This file is not used until Assignment 3
 */

#include <xeroskernel.h>
#include <xeroslib.h>

/* Your code goes here */

void sigtramp(void (*handler)(void *), void *cntx, void *old_sp) {
    // kprintf("1\n");
	// call the provided handler with the argument cntx in user space
    // kprintf("Handler in sigtramp: 0x%X\n", handler); 
    handler(cntx);
	// call sigreturn
	syssigreturn(old_sp);
}


/**
 *
 * todo: Need to be updated to indicate what to do if a read from the
 *       keyboard is partially done and a signal occurs.
 * 
 * called when a signal is to be registered for delivery to a process
 * a process can signal itself
 *
 * Unless otherwise specified using -362 as the return code of
 * system call that is interrupted/unblocked because of a signal
 *
 * Assumptions:
 * delivering to idle process is not allowed =>
 * => delivery not succesful
 * 
 * @param  pid    ID of the process to deliver the signal to
 * @param  src 	  pointer to the source of the signal
 * @param  signum signal to deliver
 * 
 * @return -1 	  if the PID is invalid
 *         -2     if the signal is invalid
 *          0     success
 */
int signal(int pid, int signum) {
	pcb *p;
    pcb *tmp;

    // sending to idleproc is invalid
    if (!pid)
        return -1;

    // check signal validity
    if (signum < 0 || signum > 31)
        return -2;

    
    // check whether a process exists and deliver a signal
    p = find(pid);
    if (!p)
    {
        return -1;
    }
    // kprintf("signal() 59: %d\n",p);

    // check if the signal is registered
    if (p && p->sigtable[signum] == 0) {
        // ignoring the signal - there's no handler registered
        return -2;
    }

    if (p->state != STATE_STOPPED) {
        
        // deliver a signal
        p->sigmask |= 1 << signum;

        // check process' state:
        // remember old state if necessary and set return values if unblocked
        // from current state
        switch(p->state) {
			case(STATE_BLOCKED_SEND):
                // set return value to send failure
				p->ret = -3;
                // remove process from queue of senders
				tmp = &proctab[p->waitingPID % MAX_PROC - 1];
				removeFromQueue(p->pid, tmp);
                // put on the readyqueue
                ready(p);
				break;
			case(STATE_BLOCKED_RECV):
				// unblocked and recv has not been completed
				p->ret = -3;
                ready(p);
			    break;
			case(STATE_SLEEPING):
				// remember the state
				p->oldstate = p->state;
				p->ret = sleep(p, 0);
                ready(p);
			    break;
            case(STATE_WAITING):
                // If a signal is targeted at the process while it is waiting
                // the signal is delivered and syswait call returns -2
                tmp = find(p->waitingPID);
                if (dequeue(p->pid, tmp->queuewaiters)) {
                    p->ret = -2;
                    p->waitingPID = 0;
                } else {
                    // something went wrong...
                    p->ret = -362;
                }
                ready(p);
                break;
			default:
                ready(p);
				// kprintf("signal() 94\n");
        }

        return 0;
    }
    return -1;
}











