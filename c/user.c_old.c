/* user.c : User processes
 */

#include <xeroskernel.h>
#include <xeroslib.h>

/**
 * Producer
 */
 void producer( void ) {
/****************************/

    syssleep(100000);
    int         i;
    char my_str[1024];
    // for (int i; i < 1000000; ++i);
    for( i = 0; i < 5; i++ ) {
        sprintf(my_str, "Produce %d\n", i );
        sysputs(my_str);
        sysyield();
    }
}

/**
 * Consumer
 */
 void consumer( void ) {
/****************************/

    syssleep(30000);
    int         i;
    char my_str[1024];
    
    // for (int i; i < 1000000; ++i);
    for( i = 0; i < 5; i++ ) {
        sprintf(my_str, "Consume %d \n", i );
        sysputs(my_str);
        sysyield();
    }
}

// empty process to test syskill
void proc1(void) {
/****************************/
    int pid;
    char my_str[1024];
    syssleep(200000);
    pid = sysgetpid();
    // for (int i; i < 1000000; ++i);
    sprintf(my_str, "Running proc1. PID: %d\n", pid);
    sysputs(my_str);
    // return;
    // for( ;; ) {
    //     // kprintf(":");
    //     sysyield();
    // }

    // for(;;);
}

// empty process to test syskill
void proc2(void) {
/****************************/
    int pid;
    char my_str[1024];
    syssleep(60000);
    pid = sysgetpid();
    // for (int i; i < 1000000; ++i);
    sprintf(my_str, "Running proc2. PID: %d\n", pid);
    sysputs(my_str);
    // return;
    // for( ;; ) {
    //     // kprintf(":");
    //     sysyield();
    // }
    // for(;;);
}

void sendTest(void) {
/****************************/
    int result;
    int pid = sysgetpid();
    //kprintf("Running sendTest. PID: %d\n", pid);
    int workPID = proctab[68 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    unsigned long num = 9;
    result = syssend(sourceprod, workPID, num);
    if(result == 0){
        kprintf("sendTest passed!\n");
    } else {
        kprintf("sendTest failed!\n");
    }
    return;
}
 
void recvTest(void) {
/****************************/
    unsigned long i = 0;
    int result;
    unsigned long *num = &i;
    int pid = sysgetpid();
    //kprintf("Running recvTest. PID: %d\n", pid);
    int* workPID = &proctab[67 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = sysrecv(sourceprod, workPID, num);
    //kprintf("SYSRECVNUMEND: %d, result %d\n", *num, result);
    if(result == 0 && *num == 9){
        kprintf("recvTest passed!\n");
    } else {
        kprintf("recvTest failed!\n");
    }
    return;
}




void recvTestFirst(void) {
/****************************/
    pcb* tmp;
    unsigned long i = 0;
    int result;
    unsigned long *num = &i;
    int pid = sysgetpid();
    //kprintf("Running recvTestFirst. PID: %d\n", pid);
    tmp = &proctab[70 % MAX_PROC-1];
    int* workPID = &tmp->pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = sysrecv(sourceprod, workPID, num);
    //kprintf("SYSRECVNUMEND: %d, result %d\n", *num, result);
    if(result == 0 && *num == 9){
        kprintf("recvTestFirst passed!\n");
    } else {
        kprintf("recvTestFirst failed!\n");
    }
    return;
}

void sendTestSecond(void) {
/****************************/
    int result;
    int pid = sysgetpid();
    //kprintf("Running sendTestSecond. PID: %d\n", pid);
    int workPID = proctab[69 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    unsigned long num = 9;
    result = syssend(sourceprod, workPID, num);
    if(result == 0){
        kprintf("sendTestSecond passed!\n");
    } else {
        kprintf("sendTestSecond failed!\n");
    }
    return;
}

void sendItselfTest(void) {
/****************************/
    unsigned long num = 0;
    int result;
    int pid = sysgetpid();
    //kprintf("Running sendItselfTest. PID: %d\n", pid);
    int workPID = proctab[pid % MAX_PROC - 1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = syssend(sourceprod, workPID, num);
    if((num == 0) && (result == -2)){
        kprintf("sendItselfTest Passed \n");
    } else {
        kprintf("sendItselfTest Failed\n");
    }
    return;
}

void recvItselfTest(void) {
/****************************/
    unsigned long i = 0;
    pcb* tmp;
    int result;
    unsigned long *num = &i;
    int pid = sysgetpid();
    //kprintf("Running recvItselfTest. PID: %d\n", pid);
    tmp = &proctab[pid % MAX_PROC - 1];
    int* workPID = &tmp->pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = sysrecv(sourceprod, workPID, num);
    if((*num == 0) && (result == -2)){
        kprintf("recvItselfTest Passed \n");
    } else {
        kprintf("recvItselfTest Failed\n");
    }
    return;
}

void send73(void) {
/****************************/
    int result;
    int pid = sysgetpid();
    //kprintf("Running sendTest73. PID: %d\n", pid);
    int workPID = proctab[75 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    unsigned long num = 8;
    result = syssend(sourceprod, workPID, num);
    if(result == 0){
        kprintf("send73 passed!\n");
    } else {
        kprintf("send73 failed!\n");
    }
    return;
}

void send74(void) {
/****************************/
    int result;
    int pid = sysgetpid();
    //kprintf("Running sendTest74. PID: %d\n", pid);
    int workPID = proctab[75 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    unsigned long num = 7;
    result = syssend(sourceprod, workPID, num);
    if(result == 0){
        kprintf("send74 passed!\n");
    } else {
        kprintf("send74 failed!\n");
    }
    return;
}

void recv75(void) {
/****************************/
    unsigned long i = 0;
    int m = 0;
    int result;
    unsigned long *num = &i;
    int pid = sysgetpid();
    //kprintf("Running recv75. PID: %d\n", pid);
    int* workPID = &m;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = sysrecv(sourceprod, workPID, num);
    if(result == 0 && *num == 8){
        kprintf("recv75 first receiveAny passed!\n");
    } else {
        kprintf("recv75 first receiveAny failed!\n");
    }
    *workPID = 0;
    result = sysrecv(sourceprod, workPID, num);
    if(result == 0 && *num == 7){
        kprintf("recv75 second receiveAny passed!\n");
    } else {
        kprintf("recv75 second receiveAny failed!\n");
    }
    return;
}

void send76(void) {
/****************************/
    int result;
    int pid = sysgetpid();
    //kprintf("Running send76. PID: %d\n", pid);
    int workPID = proctab[79 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    unsigned long num = 8;
    result = syssend(sourceprod, workPID, num);
    if(result == 0){
        kprintf("send76 passed!\n");
    } else {
        kprintf("send76 failed!\n");
    }
    return;
}

void recv77(void) {
/****************************/
    unsigned long i = 0;
    int result;
    unsigned long *num = &i;
    int pid = sysgetpid();
    //kprintf("Running recv77. PID: %d\n", pid);
    int* workPID = &proctab[78 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = sysrecv(sourceprod, workPID, num);
    if(result == 0 && *num == 7){
        kprintf("recv77 passed!\n");
    } else {
        kprintf("recv77 failed!\n");
    }
    return;
}

void send78(void) {
/****************************/
    int result;
    int pid = sysgetpid();
    //kprintf("Running send78. PID: %d\n", pid);
    int workPID = proctab[77 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    unsigned long num = 7;
    result = syssend(sourceprod, workPID, num);
    if(result == 0){
        kprintf("send78 passed!\n");
    } else {
        kprintf("send78 failed!\n");
    }
    return;
}


void recv79(void) {
/****************************/
    unsigned long i = 0;
    int result;
    unsigned long *num = &i;
    int pid = sysgetpid();
    //kprintf("Running recv79. PID: %d\n", pid);
    int* workPID = &proctab[76 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = sysrecv(sourceprod, workPID, num);
    if(result == 0 && *num == 8){
        kprintf("recv79 passed!\n");
    } else {
        kprintf("recv79 failed!\n");
    }
    return;
}

void send80(void) {
/****************************/
    int result;
    int pid = sysgetpid();
    //kprintf("Running send80. PID: %d\n", pid);
    int workPID = proctab[82 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    unsigned long num = 7;
    result = syssend(sourceprod, workPID, num);
    if(result == -1){
        kprintf("send80 passed!\n");
    } else {
        kprintf("send80 failed!\n");
    }
    return;
}

void recv81(void) {
/****************************/
    unsigned long i = 0;
    int result;
    unsigned long *num = &i;
    int pid = sysgetpid();
    kprintf("Running recv81. PID: %d\n", pid);
    int* workPID = &proctab[82 % MAX_PROC-1].pid;
    pcb* sourceprod = &proctab[pid % MAX_PROC - 1];
    result = sysrecv(sourceprod, workPID, num);
    if(result == -1){
        kprintf("recv81 passed!\n");
    } else {
        kprintf("recv81 failed!\n");
    }
    return;
}



 void     root( void ) {
/****************************/
    // a lot of testing happens here

    char my_str[1024];
    int prod_pid, con_pid, proc1_pid, proc2_pid, proc3_pid, own_pid;
    //int killres;


    own_pid = sysgetpid();
    
    sprintf(my_str, "Root has been called. PID: %d\n", own_pid);
    sysputs(my_str);


    // syssleep(60000);

    //sysputs("testing sysputs!\n");

    //sysyield();
    //sysyield();

    //prod_pid = syscreate( &producer, 4096 );
    //con_pid =  syscreate( &consumer, 4096 );
    //proc1_pid = syscreate(&proc1, 4096);





    /* 
    Send Receive Tests
    */


    // Send then receive
    syscreate(&sendTest, 4096);
    syscreate(&recvTest, 4096);

    // Receive then send
    syscreate(&recvTestFirst, 4096);
    syscreate(&sendTestSecond, 4096);

    // Send to itself test
    syscreate(&sendItselfTest, 4096);

    // Receive to itself test
    syscreate(&recvItselfTest, 4096);

    // 2 sends in a row to the same proc then recv from 2
    syscreate(&send73, 4096); // send pid: 73
    syscreate(&send74, 4096); // send pid: 74
    syscreate(&recv75, 4096); // recv pid: 75

    // Send76 to recv 79, send 78 to recv 77 in order shown below
    syscreate(&send76, 4096); // send pid: 76
    syscreate(&recv77, 4096); // recv pid: 77
    syscreate(&send78, 4096); // send pid: 78
    syscreate(&recv79, 4096); // recv pid: 79


    // Sending to a Stopped or non existent process test
    syscreate(&send80, 4096); 
    // Receiving from a stopped or non existent process test
    syscreate(&recv81, 4096);


    prod_pid = syscreate( &producer, 4096 );
    con_pid =  syscreate( &consumer, 4096 );
    proc1_pid = syscreate(&proc1, 4096);
    
    //sendTest_pid = syscreate(&sendTest, 4096);
    //recvTest_pid = syscreate(&recvTest, 4096);

    //pcb* sourceroot = &proctab[own_pid % MAX_PROC - 1];
    //kprintf("sourceroot.pid = : %d\n", sourceroot.pid);
    //kprintf("prod_pid = : %d\n", prod_pid);
    //syssend(sourceroot, prod_pid, 9);
    

    // sprintf(my_str, "Prod pid = %d; Cons pid = %d; Proc1 pid = %d; Proc3 pid = %d \n", prod_pid, con_pid, proc1_pid, proc2_pid, proc3_pid);
    // sysputs(my_str);


    /*
    PCB REUSE TESTS
     */
    // int proc2_pid, proc3_pid, proc4_pid, proc5_pid;
    // killres = syskill(prod_pid);
    // kprintf("Killed producer: %d\n", killres);
    // killres = syskill(con_pid);
    // kprintf("Killed consumer: %d\n", killres);
    // proc2_pid = syscreate( &producer, 4096 );
    // proc3_pid = syscreate( &consumer, 4096 );
    // proc4_pid = syscreate(&proc1, 4096);
    // proc5_pid = syscreate(&proc1, 4096);
    // kprintf("Proc2 = %d Proc3 = %d Proc4 = %d Proc5 = %d \n", proc2_pid, proc3_pid, proc4_pid, proc5_pid);
    
    /*
    SYSKILL TESTS
    */
    // killres = syskill(own_pid);
    // kprintf("Trying to kill itself: %d\n", killres);
    // killres = syskill(7);
    // kprintf("Trying to kill non-existent process: %d\n", killres);
    // killres = syskill(prod_pid);
    // kprintf("Killed producer: %d\n", killres);

    // proc2_pid = syscreate(&proc2, 4096);
    // proc3_pid = syscreate(&proc3, 4096);

    // sysyield();
    // sysyield();
    // sysyield();
    // sysyield();

    // killres = syskill(con_pid);
    // kprintf("Killed consumer: %d\n", killres);
    // killres = syskill(proc1_pid);
    // kprintf("Killed proc1: %d\n", killres);


    for( ;; ) {
        sysyield();
    }
    //kprintf("The End Folks");

 }
