/* disp.c : dispatcher
 */

#include <xeroskernel.h>
#include <xeroslib.h>
#include <i386.h>
#include <stdarg.h>
#include <kbd.h>

#define STARTING_EFLAGS         0x00003200

// ready queue (accessible only here)
static pcb      *head = NULL;
static pcb      *tail = NULL;

// stopped queue (accessible by many other parts of the system)
pcb      *st_head = NULL;
pcb      *st_tail = NULL;

// sleeping queue
extern pcb *sleepq;

// maximum address permitted in our system, defined in i386.c
extern char *maxaddr;

void dispatch( void ) {
/********************************/


    char            *st;

    int             pid;
    int             r;
    int             stack;
    int             ms;
    int             parent;
    int             sig;
    int             bufflen;
    int             fd;
    int             command;
    int             command2;
    int             *pid_r;

    unsigned long   num;
    unsigned long   *num_r;

    void            *newhandler;
    void            **oldhandler;
    void            *st_ptr;
    void            *buff;

    pcb             *p;
    pcb             *source;
    
    funcptr         fp;
    
    va_list         ap;



    for( p = next(); p; ) {
        // p = next() is executed only the first time, in all other iterations
        // it just checks if process exists and moves on to that process
        
        // //kprintf("1 \n");
        // if(p->pid != 66){
        //     kprintf("pointer address %x, ", p);
        //     kprintf("next proc pid: %d\n", p->pid);
        // }

        // check whether a process has pending signals
        // if true: modify stack to process the signal
        if (p->sigmask && !p->sigproc) {
            // kprintf("build stack\n");
            buildstack(p);
        }

        r = contextswitch( p );
        switch( r ) {
            case( SYS_CREATE ):
                ap = (va_list)p->args;
                fp = (funcptr)(va_arg( ap, int ) );
                stack = va_arg( ap, int );
                parent = va_arg(ap, int);
                //kprintf("FP: %d STACK: %d PARENT: %d\n", fp, stack, parent);
                p->ret = create( fp, stack, parent );
                break;
            case( SYS_YIELD ):
                ++p->timeused;
                ready( p );
                p = next();
                break;
            case( TIMER_INT ):
                tick();
                ++p->timeused;
                ready( p );
                p = next();
                end_of_intr();
                break;
            case( SYS_STOP ):
                // kprintf("Sysstop called.. Bye!");
                stop(p);
                // all cleanup in stop()
                p = next();
                break;
            case( SYS_GETPID ):
                p->ret = p->pid;
                break;
            case( SYS_GETPARENT ):
                p->ret = p->parent;
                break;
            case( SYS_KILL ):
                ap = (va_list)p->args;
                pid = va_arg(ap, int);
                sig = va_arg(ap, int);
                r = signal(pid, sig);
                // kprintf("r = %d\n", r);
                // signal delivery successful
                if (!r)
                    p->ret = 0;

                // signal number is invalid
                if (r == -2)
                    p->ret = -651;

                // target process does not exist or sending to idleproc
                if (r == -1)
                    p->ret = -712;

                break;
            case( SYS_WAIT ):
                ap = (va_list)p->args;
                pid = va_arg(ap, int);
                p->ret = wait(pid, p);
                p = next();
                break;
            case( SYS_PUTS ):
                ap = (va_list)p->args;
                st = va_arg(ap, char*);
                kprintf("%s", st);
                break;
            case( SYS_SEND ):
                ap = (va_list)p->args;
                pid = va_arg(ap, int);
                num = va_arg(ap, unsigned long);
                source = p;
                p->ret = send( pid, num, source );
                if(p->ret == -4){ // Prof said make it a bit more general
                    p = next();
                }
                break;
            case( SYS_RECV ):
                ap = (va_list)p->args;
                pid_r = va_arg(ap, int*);
                num_r = va_arg(ap, unsigned long*);
                source = p;
                p->ret = recv( pid_r, num_r, source );
                if(p->ret == -4){ // Prof said make it a bit more general
                    p = next();
                }
                break;
            case( SYS_SLEEP ):
                ap = (va_list)p->args;
                ms = va_arg(ap, unsigned int);
                p->ret = sleep(p, ms);
                p = next();
                break;
            case( SYS_QUANTUMS ):
                ap = (va_list) p->args;
                pid = va_arg(ap, int);
                p->ret = getquantumcount(pid, p->pid);
                break;
            case( SYS_OPEN ):
                //kprintf("disp Open called\n");
                ap = (va_list)p->args;
                fd = va_arg(ap, int);
                p->ret = di_open(fd, p);
                break;
            case( SYS_CLOSE ):
                //kprintf("disp Close called\n");
                ap = (va_list)p->args;
                fd = va_arg(ap, int);
                p->ret = di_close(p, fd);
                break;
            case( SYS_READ ):
                // kprintf("disp Read called\n");
                ap = (va_list)p->args;
                fd = va_arg(ap, int);
                buff = va_arg(ap, void*);
                bufflen = va_arg(ap, int);
                p->ret = di_read(p, fd, buff, bufflen);
                break;
            case( SYS_WRITE ):
                //kprintf("disp Write called\n");
                ap = (va_list)p->args;
                fd = va_arg(ap, int);
                buff = va_arg(ap, void*);
                bufflen = va_arg(ap, int);
                p->ret = di_write(p, fd, buff, bufflen);
                //kprintf("disp Write done\n");
                break;
            case( SYS_IOCTL ):
                // kprintf("disp Ioctl called\n");
                ap = (va_list)p->args;
                fd = va_arg(ap, int);
                command = va_arg(ap, unsigned long);
                command2 = va_arg(ap, int);
                // p->args = ap; // check ap passing!!!
                p->ret = di_ioctl(p, fd, command, command2); 
                break;
            case( SYS_KEYBOARD ):
                kbdInterruptHandler();
                end_of_intr();
                ready(p);
                p = next();
                break;
            case( SYS_SIGINSTALL ):
                ap = (va_list)p->args;
                sig = va_arg(ap, int);
                newhandler = va_arg(ap, void*);
                oldhandler = va_arg(ap, void**);

                if (sig < 0 || sig > 31) {
                    p->ret = -1;
                }
                else if (!checkaddr(newhandler)) {
                    p->ret = -2;
                }
                else {
                    *oldhandler = p->sigtable[sig];
                    p->sigtable[sig] = newhandler;
                    p->ret = 0;
                }
                break;
            case( SYS_SIGRETURN ):
                //kprintf("4\n");
                ap = (va_list) p->args;
                st_ptr = va_arg(ap, void*);
                p->esp = (unsigned long *) st_ptr;
                // recover p->ret from stack after sigreturn
                p->ret = *(p->esp - 1);
                if (p->oldstate == STATE_SLEEPING) {
                    sleep(p, p->ret);
                    p = next();
                }
                p->sigproc = 0;
                ready(p);
                p = next();
                break;
            default:
                kprintf( "Bad Sys request %d, pid = %d\n", r, p->pid );
                // Dump registers
                trap(3);
        }
        if (!p)
            p = &proctab[0];
    }
    kprintf( "Out of processes: dying\n" );
    for( ;; );
}

extern void dispatchinit( void ) {
/********************************/

  //bzero( proctab, sizeof( pcb ) * MAX_PROC );
  memset(proctab, 0, sizeof( pcb ) * MAX_PROC);
  head = &proctab[0];
}


/**
 * IDLE process
 */
void idleproc(void) {
/****************************/
    for(;;){
        __asm __volatile( " \
            hlt \n\
        "
        : 
        : 
    );
    }
}


/**
 * Removes a process identified by PID from one of 2 queues:
 * - sleeping
 * - ready
 *
 * @param toRemove  pid of the process to be dequeued
 * @param p         head of the queue from which to dequeue
 *
 * @return 1    success
 *         0    failure
 */
extern int dequeue( int toRemove, pcb* p ){
    pcb *prev;
    prev = NULL;

    if (!toRemove) {
        return 0;
    }

    while (p->pid != toRemove || !p) {
        prev = p;
        p = p->next;
    }

    if (!p) {
        return 0;
    }

    if (!prev) {
        if (p == sleepq)
            sleepq = sleepq->next;
        else if (p == head)
            head = head->next;
    } else {
        prev->next = p->next;
    }
    p->next = NULL;
    return 1;
}


// put a process on the readyqueue
extern void ready( pcb *p ) {
/*******************************/
    p->next = NULL;
    p->state = STATE_READY;
    // if(p->pid != 66){
    //     kprintf("process in ready queue: %d\n", p->pid);
    // }
    if( tail ) {
        tail->next = p;
    } else {
        head = p;
    }

    tail = p;
}



/**
 * 1. dequeue a process from a queue it is on
 * 2. put a process on the stopped queue
 * SUCCESS: return 1;
 * FAIL: return 0.
 */
extern int stop(pcb *p) {
/*******************************/
    pcb *tmp;
    int result = 0;

    // choose a queue to dequeue from
    switch (p->state) {
        case(STATE_READY):
            result = dequeue(p->pid, head);
            break;
        case(STATE_SLEEPING):
            result = dequeue(p->pid, sleepq);
            break;
        case(STATE_BLOCKED_SEND):
            tmp = &proctab[p->waitingPID % MAX_PROC - 1];
            // calling a function to dequeue from senders queue in msg.c
            result = removeFromQueue(p->pid, tmp);
            break;
        case(STATE_BLOCKED_RECV):
            result = 1;
            p->next = NULL;
            break;
        case(STATE_RUNNING):
            result = 1;
            break;
        case(STATE_WAITING):
            tmp = find(p->syswaitpid);
            result = dequeue(p->pid, tmp->queuewaiters);
            break;
        default:
            kprintf("UNKNOWN CASE!\n");
            trap(3);
    }
    
    if (!result)
        return result;

    // put a process on the stopped queue
    if (!st_head) {
        st_head = p;
    } else {
        st_tail->next = p;
    }
    st_tail = p;
    st_tail->next = NULL;

    p->state = STATE_STOPPED;
    p->esp = 0;
    p->next = 0;
    p->ret = 0;
    p->interr = 0;
    p->alarm = 0;
    p->buffer = 0;
    p->args = 0;
    p->timeused = 0;

    result = kfree(p);

    // notify senders that you died
    tmp = p->queueSenders;
    while (tmp) {
        tmp->waitingPID = 0;
        tmp->ret = -1;
        ready(&(*tmp));
        tmp = tmp->next;
    }

    // notify receivers that you died
    for (int i = 1; i < MAX_PROC; ++i)
    {
        if (proctab[i].waitingPID == p->pid)
        {
            proctab[i].waitingPID = 0;
            proctab[i].ret = -1;
            ready(&proctab[i]);
        }
    }

    // notify waiters that you're dying
    // kprintf("disp.c 380: WTF?!\n");
    tmp = p->queuewaiters;
    while (tmp) {
        pcb *t;
        // kprintf("disp.c 383: WTF?!\n");
        tmp->syswaitpid = 0;
        tmp->ret = 0;
        t = tmp;
        tmp = tmp->next;
        ready(&(*t));
    }

    return result;
}


/**
 * Finds a process by given PID and returns a pointer to a pcb
 * SUCCESS: pointer
 * FAIL: null pointer
 */
extern pcb* find(int pid) {
/*****************************/

    for (int i = 0; i < MAX_PROC; ++i)
    {
        if (pid == proctab[i].pid) {
            return &proctab[i];
        }
    }
    return NULL;
}


// return next process from the ready queue
extern pcb* next( void ) {
/*****************************/
    pcb *p;


    p = head;
    if( p ) {
        head = p->next;
        if( !head ) {
            tail = NULL;
        }
    }
    p->state = STATE_RUNNING;
    return( p );
}



/**
 * Returns the number of ticks used by a process with a given pid
 * DNE stands for does not exist
 * 
 * @param  req_pid: pid to show quantum count for
 * @param  pid: pid of the process requesting quantum count
 * 
 * @return int number of quantums used by a process
 * @return -1 if a process does not exist
 */
int getquantumcount(int req_pid, int pid) {
    pcb* p;

    // return quantum count for self
    if (req_pid == -1) {
        p = find(pid);
        return p->timeused;
    }
    
    // find a process with requested pid and return quantum count or -1 if DNE
    p = find(req_pid);
    
    if (!p)
        return -1;

    if (p->state != STATE_STOPPED)
        return p->timeused;
    
    return -1;
}


/**
 * checks if address pointed to by a function pointer is valid
 * @param  addr - address of a function
 * @return 1 if address is within the boundaries
 *         0 in all other cases
 */
int checkaddr(void* addr) {
    // check if it is greater than or equal to zero
    if (!addr) {
        return 0;
    }
    // check if it is bigger than the maximum address-4
    if ((char *) addr > maxaddr) {
        return 0;
    }
    // check if it is in the hole
    if ((char *) addr > (char *) HOLESTART && (char *) addr < (char *) HOLEEND)
    {
        return 0;
    }
    return 1;
}



/**
 * modifies process stack so that it runs sigtramp code
 * when processing a signal
 * 
 * @param  p - pointer to a process' pcb
 * @return true - if success
 *         false - if no success
 */
void buildstack(pcb* p) {
    int                 count;
    unsigned int        mask;
    unsigned long       *new_esp;
    context_frame       *cf;

    count = 31;
    mask = 1 << 31; // set the 31-st bit to 1
    
    // finding which bit is set (left to right) gives us a signal code
    while (!(mask & p->sigmask) || count < 0) {
        --count;
        mask = mask >> 1;
    } // now count is our signal code
    
    // resetting signal bit to 0
    p->sigmask &= ~(1 << count);
    
    // kprintf("buildstack old_sp: 0x%X\n", p->esp);

    new_esp = p->esp;
    --new_esp;

    // kprintf("new_esp: 0x%X\n", new_esp);

    // push p->ret onto stack
    *new_esp = p->ret;
    --new_esp;

    // kprintf("new_esp: 0x%X\n", new_esp);

    // save old esp on stack
    *new_esp = (unsigned long) p->esp;
    --new_esp;

    // kprintf("new_esp: 0x%X\n", new_esp);
    
    // save a pointer to context frame on stack
    // has to be a pointer to the old context frame
    // so that the handler can modify some values in it
    *new_esp = (unsigned long) p->esp;
    --new_esp;

    // kprintf("new_esp: 0x%X\n", new_esp);

    // save a pointer to handler on stack
    *new_esp = (unsigned long) *(p->sigtable[count]);
    --new_esp;

    // kprintf("new_esp: 0x%X\n", new_esp);

    // set a return address we don't care about
    *new_esp = 0x66;

    // push CF onto stack
    cf = (context_frame *) ((unsigned char *) new_esp
        - sizeof(context_frame) + 4);

    // kprintf("cf: 0x%X, %d\n", cf, sizeof(context_frame));
    
    // set eip to &sigtramp and set flags
    cf->iret_eip = (unsigned long) sigtramp;
    cf->iret_cs = getCS();
    cf->eflags = STARTING_EFLAGS;
    // new_esp->rett = sysstop;

    // replace old stack pointer with the new one
    p->esp = (unsigned long *) cf;
    p->sigproc = 1;
    // kprintf("build stack: complete\n");

}


/**
 * Causes the calling process to wait for the process with the given PID to terminate.
 *
 * Assumption: cannot wait for idle process to die, returning -1
 * 
 * @param pid   process id to wait for
 *
 * @return  0   success
 *         -1   process does not exist
 *
 */
int wait(int pid, pcb* p) {
    pcb *tmp;
    tmp = find(pid);

    //kprintf("disp.c 582: waiting for pid: %d tmp: 0x%X\n", pid, tmp);

    // if process doesn't exist
    // or pid == 0
    if (!tmp || pid == 0)
        return -1;

    // put a process on a waiting queue
    if (!tmp->queuewaiters) {
        tmp->queuewaiters = p;
    } else {
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = p;
    }
    p->next = NULL;
    p->state = STATE_WAITING;
    p->syswaitpid = pid;

    //kprintf("waitingpid: %d p-state: %d queuewaiters: %x\n", p->syswaitpid, p->state, tmp->queuewaiters);

    return 0;
}






