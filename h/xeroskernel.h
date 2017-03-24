/* xeroskernel.h - disable, enable, halt, restore, isodd, min, max */

#ifndef XEROSKERNEL_H
#define XEROSKERNEL_H

/* Symbolic constants used throughout Xinu */

typedef	char    Bool;        /* Boolean type                  */
typedef unsigned int size_t; /* Something that can hold the value of
                              * theoretical maximum number of bytes 
                              * addressable in this architecture.
                              */
                             
/* A typedef for the signature of the function passed to syscreate */
typedef void    (*funcptr)(void);

#define	FALSE   0       /* Boolean constants             */
#define	TRUE    1
#define	EMPTY   (-1)    /* an illegal gpq                */
#define	NULL    0       /* Null pointer for linked lists */
#define	NULLCH '\0'     /* The null character            */

#define CREATE_FAILURE -1  /* Process creation failed     */ 



/* Universal return constants */

#define	OK            1         /* system call ok               */
#define	SYSERR       -1         /* system call failed           */
#define	EOF          -2         /* End-of-file (usu. from read)	*/
#define	TIMEOUT      -3         /* time out  (usu. recvtim)     */
#define	INTRMSG      -4         /* keyboard "intr" key pressed	*/
                                /*  (usu. defined as ^B)        */
#define	BLOCKERR     -5         /* non-blocking op would block  */

/* Functions defined by startup code */


void           bzero(void *base, int cnt);
void           bcopy(const void *src, void *dest, unsigned int n);
void           disable(void);
unsigned short getCS(void);
unsigned char  inb(unsigned int);
void           init8259(void);
int            kprintf(char * fmt, ...);
void           lidt(void);
void           outb(unsigned int, unsigned char);


/* Some constants involved with process creation and managment */
 
   /* Maximum number of processes */      
#define MAX_PROC        64
   // Maximum number of devices      ****************
#define MAX_DEV         4
   /* Max PID value */
#define MAX_PID         128
   /* Kernel trap number (used for syscalls) */
#define KERNEL_INT      80
   /* Timer interrrupt number */
#define TIMER_INT       32
#define KEYBOARD_INT    33

   /* Minimum size of a stack when a process is created */
#define PROC_STACK      (4096 * 4)    

/* Divisor for initPIT */
#define DIVISOR         100

/* Constants to track states that a process is in */
#define STATE_STOPPED           0
#define STATE_READY             1
#define STATE_BLOCKED_SEND      2
#define STATE_BLOCKED_RECV      3
#define STATE_SLEEPING          4
#define STATE_RUNNING           5
#define STATE_WAITING           6


/* System call identifiers */
#define SYS_STOP        10
#define SYS_YIELD       11
#define SYS_CREATE      22
#define SYS_TIMER       33
#define SYS_QUANTUMS    34
#define SYS_SIGINSTALL  35
#define SYS_KEYBOARD    36
#define SYS_SIGRETURN   37
#define SYS_GETPID      44
// Device SYS call nums
#define SYS_OPEN        45
#define SYS_CLOSE       46
#define SYS_READ        47
#define SYS_WRITE       48
#define SYS_IOCTL       49
// ******
#define SYS_PUTS        55
#define SYS_SEND        66
#define SYS_RECV        77
#define SYS_SLEEP       88
#define SYS_KILL        90
#define SYS_WAIT        91
#define SYS_GETPARENT   99

#define RECV_ANY        0




// Device stuff *******************
#define CONSOLE    0
#define SERIAL0    1
#define SERIAL1    2
#define KBMON      3
#define TTY0       4
#define KBD        5

// KBD commands
#define KBD_NONECHO 55
#define KBD_ECHO    56

// Device struct *****************

// **** Define file descriptor entry

typedef struct device device;
struct device {
  int dvnum;
  char *dvname;
  int (*dvinit)(void);
  void (*dvopen)(void);
  void (*dvclose)(void);
  int (*dvread)( unsigned char* buff, int bufflen);
  int (*dvwrite)( unsigned char*, int bufflen);
  int (*dvioctl)(unsigned long command, int command2);

  int (*dvgetc)(void);
  int (*dvputc)(void);
  int (*dvcntl)(void);
  void *dvcsr;
  void *dvivec;
  void *dvovec;
  int (*dviint)(void);
  int (*dvoint)(void);
  void *dvioblk;
  int dvminor;
};

// Device Table
device devtab[MAX_DEV];


/* Structure to track the information associated with a single process */

typedef struct struct_pcb pcb;
struct struct_pcb {
  pcb                 *queueSenders;
  pcb                 *next;      // Next process in the list, if applicable
  pcb                 *queuewaiters; // queue of those who wait for you to die
  unsigned long long  alarm;
  unsigned long       *bufferPointer; // num pointer to save value in
  unsigned long       *esp;       // Pointer to top of saved stack
  long                args;
  funcptr             sigtable[32]; // signal table for the process
  unsigned int        sigmask;    // signal mask
  unsigned int        sigproc;
  int                 syswaitpid; // store pid of who you are waiting to die
  int                 parent;
  int                 state;      // State the process is in, see above
  int                 oldstate;   // state before signal
  int                 waitingPID; // the process currently waiting for
  int                 interr;
  int                 pid;        // The process's ID
  int                 ret;        // Return value of system call
  int                 buffer;     // buffer for send() and receive()
  int                 timeused;   // number of ticks used by a process
  device              *fdt[4];     // file descriptor table
};


/* The actual space is set aside in create.c */
extern pcb     proctab[MAX_PROC];

// stopped queue shared with other parts of the system
extern pcb     *st_head;
extern pcb     *st_tail;

// // ready queue
// extern pcb     *head;
// extern pcb     *tail;

// sleeping queue
extern pcb    *sleepq;

#pragma pack(1)

/* What the set of pushed registers looks like on the stack */
typedef struct context_frame {
  unsigned long        edi;
  unsigned long        esi;
  unsigned long        ebp;
  unsigned long        esp;
  unsigned long        ebx;
  unsigned long        edx;
  unsigned long        ecx;
  unsigned long        eax;
  unsigned long        iret_eip;
  unsigned long        iret_cs;
  unsigned long        eflags;
  funcptr              rett;
  unsigned long        stackSlots[];
} context_frame;







/* Memory mangement system functions, it is OK for user level   */
/* processes to call these.                                     */

int       kfree( void *ptr );
void      kmeminit( void );
void      *kmalloc( size_t );
void      kpcbinit( void );


/* Internal functions for the kernel, applications must never  */
/* call these.                                                 */
void      dispatch( void );
void      dispatchinit( void );
int       dequeue( int toRemove, pcb* p );
int       stop(pcb *p);
pcb*      find(int pid);
void      ready( pcb *p );
pcb*      next( void );
void      contextinit( void );
int       contextswitch( pcb *p );
int       create( funcptr fp, size_t stack , int parent );
int       idlecreate(void);
// int       kill(int tokill, pcb *killer, int sigNum);
int       signal(int pid, int signum);
int       getquantumcount(int req_pid, int pid);
void      set_evec(unsigned int xnum, unsigned long handler);
void      printCF (void * stack);  /* print the call frame */
int       syscall(int call, ...);  /* Used in the system call stub */
int       checkaddr(void* addr);     /* check func address validity */
void      buildstack(pcb* p);      /*build stack for signal procesing*/
int       wait(int pid, pcb* p);   // put a process on the waiting queue
void      trap(int inum);
void      keyboardInit(void);



/* Function prototypes for system calls as called by the application */
// returns pid of new process
int       syscreate( funcptr fp, size_t stack, int parent );
void      sysyield( void );
void      sysstop( void );
int       sysgetpid(void);              // return pid of current proc
int       syskill(int pid, int sigNum);             // kill a process with pid
void      sysputs(char *str);
int       syssleep(unsigned int ms);    // return time left to sleep
int       sysgetparent(void);           // return creator pid
int       sysgetcputime(int pid);       // return number of quantums used
int       syssighandler( int signal, void (*newhandler)(void *),
              void (** oldHandler)(void *) ); // install signal handler
void      syssigreturn(void *old_sp);   // restore process esp after sigtramp
int       syswait(int pid);             // wait till pid dies

// trampoline function
void      sigtramp(void (*handler)(void *), void *cntx, void *old_sp);

// Device syscalls
int       sysopen(int deviceNum);                     // Device Open
int       sysclose(int fd);                           // Device Close
int       syswrite(int fd, void *buff, int bufflen);  // Device Write
int       sysread(int fd, unsigned char *buff, int bufflen);   // Device Read
int       sysioctl(int fd, unsigned long command, int command2);  // Device specific stuff ioctl

// Device Independent calls
int       di_open(int deviceNum, pcb* p);
int       di_close(pcb* p, int fd);
int       di_read(pcb* p, int fd, unsigned char *buff, int bufflen);
int       di_write(pcb* p, int fd, unsigned char *buff, int bufflen);
int       di_ioctl(pcb* p, int devid, unsigned long command, int command2);

// IPC System Calls
int       syssend( int dest_pid, unsigned long num );
int       sysrecv( int* from_pid, unsigned long* num );
int       removeFromQueue( int elemToRemove, pcb* source );

// Kernel side of IPC System calls
int       send( int dest_pid, unsigned long num, pcb* source );
int       recv( int *from_pid, unsigned long *num, pcb* source );

// Testing processes (Uncomment one of these in init.c to run the tests)
void      root(void);   // initial process that the system creates
void      root2(void);
void      root3(void);
void      root4(void);
void      root5(void);
void      rootFinal(void); // Final Testing, 2.7 of assignment 3
void      getquantum_master_test(void);   // Testing sysgetcputime()
void      keyboardInitTest(void);
void      keyboardExtraTest(void);
void      sig_test(void);  // testing signals
void      wait_test(void); // testing syswait

/* idle process */
void      idleproc(void);

/* Sleep and tick */
int       sleep(pcb* p, unsigned int ms);
void      tick(void);

void      set_evec(unsigned int xnum, unsigned long handler);


// Device independent functions **************
extern int ttyinit(void);
extern int ttyopen(void);
extern int ionull(void);
extern int ttyread(void);
extern int ttywrite(void);
extern int ioerr(void);



/* Anything you add must be between the #define and this comment */
#endif

