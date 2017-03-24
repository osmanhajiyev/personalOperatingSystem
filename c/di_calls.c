// di_calls.c
#include <xeroskernel.h>
#include <xeroslib.h>
#include <stdarg.h>



// UPPER HALF DEVICE OPEN: does the upper half of opening and calls lower half open
int di_open(int deviceNum, pcb* p){
	int i = 0;
	// kprintf("di_open Called\n");
    device* devptr = &devtab[deviceNum];
    if(devptr->dvopen == NULL) {
    	kprintf("di_open failed\n");
    	return -1;
    }
    while(p->fdt[i] != NULL){
        i++;
    }
    if(i<=MAX_DEV-1){
    	//kprintf("i = %d\n", i);
    	(devptr->dvopen)();
        p->fdt[i] = devptr;
        //kprintf("fdt num = %d\n", p->fdt[i]->dvnum);
    } else {
        i = -1;
    }
    //kprintf("di_open finished result %d\n", i);
    return i;
}

// UPPER HALF DEVICE CLOSE: does the upper half of closing and calls lower half close
int di_close(pcb* p, int fd){
	//kprintf("di_close Called\n");
    device* devptr = p->fdt[fd];
    if ( !(fd <= MAX_DEV-1)  && (devptr->dvclose != NULL)) return SYSERR; // maybe it should be the first line?
    (devptr->dvclose)();
    p->fdt[fd] = NULL;
    return 0;
}

// UPPER HALF DEVICE READ: does the upper half of reading and checking and calls lower half read
int di_read(pcb* p, int fd, unsigned char *buff, int bufflen) {
	//kprintf("di_read Called \n");
    device *devptr = p->fdt[fd]; 
    if ( !(fd <= MAX_DEV-1)  && (devptr->dvread != NULL)) return SYSERR;
    return (devptr->dvread)(buff, bufflen); // no need for devptr
}

// UPPER HALF DEVICE WRITE: does the upper half of writing and checking and calls lower half write
int di_write(pcb* p, int fd, unsigned char *buff, int bufflen) {
	//kprintf("di_write Called \n");
    device *devptr = p->fdt[fd];
    //kprintf("di_write dvnum =  %d\n", devptr->dvnum);
    if ( !(fd <= MAX_DEV-1) && (devptr->dvwrite != NULL)) return SYSERR;
    //kprintf("di_write Called3 \n");
    return (devptr->dvwrite)(buff, bufflen);
}

// UPPER HALF DEVICE IOCTL: does the upper half of ioctl and checking and calls lower half ioctl
int di_ioctl(pcb* p, int fd, unsigned long command, int command2){
	//kprintf("di_ioctl Called \n");
	device *devptr = p->fdt[fd];
	//va_list ap = (va_list)p->args;
    // fixme whats does the next line do?
    if ( !(fd <= MAX_DEV-1) && (devptr->dvwrite != NULL)) return SYSERR;
    return (devptr->dvioctl)(command, command2);
}