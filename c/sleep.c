/* sleep.c : sleep device 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>
#include <xeroslib.h>

/**
 * SYSTEM TIME
 * - counts system time since its boot up
 * - uses long long because int limits the tick coiunter to only 470+ days of
 *   running time
 */
static unsigned long long systime;

pcb* sleepq;

/**
 * put a process on the sleep queue
 */
static void put(pcb* p) {
    pcb* tmp = sleepq;
    pcb* prev;

    // if sleepqueue empty
    if (!tmp) {
        sleepq = p;
        sleepq->next = NULL;
        sleepq->state = STATE_SLEEPING;
    }

    // insert in the middle or at the end
    else {
        while (tmp) {
            //kprintf("TMP->ALRM: %d; P->ALRM: %d\n", tmp->alarm, p->alarm);
            if (p->alarm > tmp->alarm && tmp->next) {
                prev = tmp;
                tmp = tmp->next;
            } else {
                break;
            }
        }
        if (tmp == sleepq && p->alarm < tmp->alarm) {
            p->next = sleepq;
            sleepq = p;
            p->state = STATE_SLEEPING;
        }
        else if (p->alarm < tmp->alarm) {
            // kprintf("PREV->ALRM: %d; P->ALRM: %d TMP->ALARM: %d\n", prev->alarm, p->alarm, tmp->alarm);
            prev->next = p;
            p->next = tmp;
            p->state = STATE_SLEEPING;
            // for(;;);
        } else {
            p->next = tmp->next;
            tmp->next = p;
            p->state = STATE_SLEEPING;
        }
    }

    // // print the queue:
    // tmp = sleepq;
    // int z = 0;
    // while (tmp && z < 10) {
    //     kprintf("%d->", tmp->pid);
    //     tmp = tmp->next;
    //     ++z;
    // }
    // if (!tmp)
    //     kprintf("NULL\n");

}


/**
 * pop a process from the sleep queue and add it to the ready queue
 */
static void wakeup(pcb *p) {
    // kprintf("WAKING UP: %d SYSTIME: %d\n", p->pid, systime);
    pcb* tmp;
    tmp = sleepq;
    
    sleepq = tmp->next;
    tmp->alarm = 0;
    ready(tmp);
}


/**
 * Put a process to sleep... sweet dreams!
 *
 * @param *p   a pointer to process pcb
 * @param ms   number of milliseconds to sleep (ms > 0)
 *             a "signal" to wake up earlier (ms == 0)
 *
 * @return 0   process put to sleep succesfully
 *         >0  remaining sleeping time (milliseconds)
 */
int sleep(pcb* p, unsigned int ms) {
    int ticks;

    // wake the process earlier if ms == 0
    // return remaining sleeping time
    if (!ms) {
        int time;
        dequeue(p->pid, sleepq);
        time = (p->alarm - systime) * DIVISOR;
        kprintf("sleep 108: Woke up earler... %d\n", time);
        return time;
    }

    kprintf("sleep 112: going to sleep... %d\n", ms);

    ticks = ms/DIVISOR;
    p->alarm = systime + ticks;
    // put a process on a sleep queue
    put(p);
    return 0;
}


/**
 * Count time and call a wake up function. Time to work!
 */
void tick(void) {
    ++systime;
    while (sleepq) {
        if (sleepq->alarm <= systime) {
            sleepq->ret = 0;
            wakeup(sleepq);
        } else {
            break;
        }
    }
}


