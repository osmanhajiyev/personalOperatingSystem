/* create.c : create a process
 */

#include <xeroskernel.h>
#include <xeroslib.h>

// process table
pcb     proctab[MAX_PROC];

/* make sure interrupts are armed later on in the kernel development  */
#define STARTING_EFLAGS         0x00003200

// PIDs can't start at 0 nor can they be negative
static int      nextpid = 1;

int idlecreate(void) {
    context_frame       *cf;
    pcb                 *p = &proctab[0];

    cf = kmalloc( PROC_STACK );
    if( !cf ) {
        return 0;
    }

    cf = (context_frame *)((unsigned char *)cf + PROC_STACK - 4);
    cf--;

    memset(cf, 0xA5, sizeof( context_frame ));

    cf->iret_cs = getCS();
    cf->iret_eip = (unsigned int) idleproc;
    cf->eflags = STARTING_EFLAGS;

    // sysstop on return
    cf->rett = sysstop;

    cf->esp = (int)(cf + 1);
    cf->ebp = cf->esp;
    p->esp = (unsigned long*)cf;
    p->state = STATE_READY;
    p->pid = 0;
    p->timeused = 0;
    p->sigmask = 0;

    return 1;
}



int create( funcptr fp, size_t stackSize, int parent ) {
/***********************************************/

    context_frame       *cf;
    pcb                 *p = NULL;
    // int                 i;


    /* PID has wrapped and we can't have -ve numbers 
     * this means that the next PID we handout can't be
     * in use. To find such a number we have to propose a 
     * new PID and then scan to see if it is in the table. If it 
     * is then we have to try again. We know we will succeed 
     * because the process table size is smaller than PID space.
     * However, this code does not do that and just returns an 
     * error.
     */


    if (nextpid < 0) 
      return CREATE_FAILURE;

    // If the stack is too small make it larger
    if( stackSize < PROC_STACK ) {
        stackSize = PROC_STACK;
    }

    // kprintf("ST_HEAD  NEXT: %x %x\n", st_head, proctab[0].next);
    
    // return a PCB in constant time from a stopped queue
    if (st_head) {
        p = st_head;
        // kprintf("52 st_head->next: %x \n", st_head->next);
        st_head = st_head->next;
        // kprintf("next: %x\n", st_head);
        p->next = NULL;
    } else {
        kprintf("NO PROCESS TABLE ENTRIES LEFT\n");
    }
    
    //    Some stuff to help wih debugging
    //    char buf[100];
    //    sprintf(buf, "Slot %d empty\n", i);
    //    kprintf(buf);
    //    kprintf("Slot %d empty\n", i);
    
    if( !p ) {
        return CREATE_FAILURE;
    }


    cf = kmalloc( stackSize );
    if( !cf ) {
        return CREATE_FAILURE;
    }

    cf = (context_frame *)((unsigned char *)cf + stackSize - 4);
    cf--;

    memset(cf, 0xA5, sizeof( context_frame ));

    cf->iret_cs = getCS();
    cf->iret_eip = (unsigned int)fp;
    cf->eflags = STARTING_EFLAGS;

    // sysstop on return
    cf->rett = sysstop;

    cf->esp = (int)(cf + 1);
    cf->ebp = cf->esp;
    p->esp = (unsigned long*)cf;
    p->state = STATE_READY;
    p->pid = p->pid + MAX_PROC;
    p->parent = parent;
    p->timeused = 0;
    p->sigmask = 0;
    p->queuewaiters = 0;
    p->waitingPID = 0;
    p->sigproc = 0;

    // set signal table to ignore the signals by default
    for (int i = 0; i < (sizeof(p->sigtable)/4); ++i)
    {
        p->sigtable[i] = NULL;
    }

    // set signal 9 to sysstop
    p->sigtable[9] = sysstop;

    ready( p );
    return p->pid;
}
