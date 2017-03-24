/* syscall.c : syscalls
 */

#include <xeroskernel.h>
#include <stdarg.h>


int syscall( int req, ... ) {
/**********************************/

    va_list     ap;
    int         rc;

    va_start( ap, req );

    __asm __volatile( " \
        movl %1, %%eax \n\
        movl %2, %%edx \n\
        int  %3 \n\
        movl %%eax, %0 \n\
        "
        : "=g" (rc)
        : "g" (req), "g" (ap), "i" (KERNEL_INT)
        : "%eax" 
    );
 
    va_end( ap );
    return( rc );
}

int syscreate( funcptr fp, size_t stack, int parent) {
/*********************************************/

    return( syscall( SYS_CREATE, fp, stack, parent ) );
}

void sysyield( void ) {
/***************************/
  syscall( SYS_YIELD );
}

void sysstop( void ) {
/**************************/
    // // testing sysstop on return
    // kprintf("CALLED SYSTOP!\n");
    syscall( SYS_STOP );
}

/*********** our code begins here ***********/

// returns the PID of the current process
int sysgetpid(void) {
    return (syscall( SYS_GETPID ));
}

// returns the PID of the processes creator
int sysgetparent(void) {
    return (syscall( SYS_GETPARENT ));
}

/**
 * Difference between syskill and sysstop?
 * 
 * sysstop: stops a process when it requests or returns
 * syskill: one process is killing another & cannot kill itself
 */

// terminates the process indentified by pid.
int syskill(int pid, int sigNum) {
    return (syscall( SYS_KILL, pid, sigNum ));
}

// used by processes to perform output.
void sysputs(char *str) {
    // Takes a pointer to a null terminated (\0) string
    // Kernel side uses kprintf() to print the string
    syscall( SYS_PUTS, str );
}

// the syssend locks until the operation completes
// returns 0 on successful send operation
// returns -1 no proc no longer exists
// if sending to itself then return -2
// for other errors returns -3
int syssend( int dest_pid, unsigned long num ){
    return (syscall( SYS_SEND, dest_pid, num ));
}

// the sysrecv locks until the operation completes
// returns 0 if message received
// -1 PID to receive from is invalid
// -2 if receive to itself
// -3 for all other errors (invalid buffer address)
int sysrecv( int *from_pid, unsigned long * num){
    return (syscall( SYS_RECV, from_pid, num ));
}

// syssleep -- sweet dreams, dear process... zZzZ...
int syssleep(unsigned int ms) {
    return ( syscall( SYS_SLEEP, ms ) );
}


// returns number of CPU quantums used by a process with a given PID
int sysgetcputime(int pid) {
    return ( syscall(SYS_QUANTUMS, pid) );
}

/**
 * Opens a device with a given device number
 * 
 * @param  deviceNum device number registered in the system
 * @return -1        open failed
 *          0...3    on success: file descriptor for the opened device
 */ 
int sysopen(int deviceNum){
    return ( syscall(SYS_OPEN, deviceNum) );
}

/**
 * closes a file descriptor from previously successful open.
 * 
 * @param  fd file descriptor
 * @return  0 success
 *         -1 failure
 */
int sysclose(int fd){
    return ( syscall(SYS_CLOSE, fd) );
}

// Performs a write operation with given buffer to a given descriptor
int syswrite(int fd, void *buff, int bufflen){
    return ( syscall(SYS_WRITE, fd, buff, bufflen) );
}

// Performs a read operation with given buffer to a given descriptor
int sysread(int fd, unsigned char *buff, int bufflen){
    return ( syscall(SYS_READ, fd, buff, bufflen) );
}

/**
 * Executes the specified control command. The action taken is device specific
 * and depends upon the control command.
 * Additional parameters are device specific.
 * @param  fd       file descriptor
 * @param  command  device specific
 * @param  command2 device specific
 * @return  0       success
 *         -1       fail
 */
int sysioctl(int fd, unsigned long command, int command2){
    return ( syscall(SYS_IOCTL, fd, command, command2) );
}

/**
 * register the given signal number as a handler
 * invalid signal: return -1
 * handler at invalid address: return -2
 * handler succesfully installed: return 0
 *
 * func registered must take only one argument
 * 
 */
int syssighandler(int signal, void (*newhandler)(void *), void (** oldHandler)(void *)) {
    return ( syscall(SYS_SIGINSTALL, signal, newhandler, oldHandler) );
}


/**
 * restore process esp after handling signal with sigtramp
 * @param  old_sp old stack pointer (need to restore process' esp to old_sp)
 */
void syssigreturn(void *old_sp) {
    syscall(SYS_SIGRETURN, old_sp);
}


/**
 * enforce calling process to wait for the process with given pid to terminate
 *
 * @param pid process id to wait for
 * 
 * @return 0 success
 * @return -1 process to be waited for does not exist
 * @return -2 targeted at the process that is waiting, signal delivered
 */
int syswait(int pid) {
    return ( syscall(SYS_WAIT, pid) );
}



