/* mem.c : memory manager
 */

#include <xeroskernel.h>
#include <i386.h>

/* Your code goes here */
/*Why do you need a paragraph mask?*/
#define PARAGRAPH_MASK  (~(0xf))

extern long     freemem;

typedef struct struct_mem mem;
struct struct_mem {
    mem         *next;
    mem         *prev;
    int         size;
};

static mem      *head;

 void kmeminit( void ) {
/****************************/

     long       s;

     s = ( freemem + 0x10 ) & PARAGRAPH_MASK;

     head = (mem *)s;
     head->size = HOLESTART - s;
     head->prev = NULL;

     s = HOLEEND;

     head->next = (mem *)s;
     head->next->next = NULL;
     head->next->prev = head;
     head->next->size = (1024 * 1024 * 4) - HOLEEND;
}

int kfree(void * mem) {
  return 1;
}


/**
 * Initialize pcb table and put all processes on stopped queue
 */
void kpcbinit( void ) {
    // Init process table
    for (int i = 1; i < MAX_PROC; ++i)
    {
        // set initial pids in process table
        proctab[i].pid = i+1;
        // add to stopped queue
        proctab[i].state = STATE_STOPPED;
        if (st_head == NULL) {
            st_tail = &proctab[i];
            st_head = &proctab[i];
            st_tail->next = NULL;
            // kprintf("head tail: %x %x \n", st_head, st_tail);
        } else {
            st_tail->next = &proctab[i];
            st_tail = &proctab[i];
            //st_tail->next = NULL;
            // kprintf("head->next tail: %x %x \n", proctab[0].next, proctab[i].next);
        }
    }
}


 void *kmalloc( size_t size ) {
/********************************/

    mem         *p;
    mem         *r;

    if( size & 0xf ) {
        size = ( size + 0x10 ) & PARAGRAPH_MASK;
    }

    for( p = head; p && ( p->size < size ); p = p->next );
    
    if( !p ) {
        return( 0 );
    }

    if( ( p->size - size ) < sizeof( mem ) ) {
       if( p->next ) {
            p->next->prev = p->prev;
        }

        if( p->prev ) {
            p->prev->next = p->next;
        } else {
            head = p->next;
        }
    } else {
        r = (mem *) ( (int)p + size );
        *r = *p;

        if( p->next ) {
            p->next->prev = r;
        }

        if( p->prev ) {
            p->prev->next = r;
        } else {
            head = r;
        }
    }

    return( p );
}
