/* user.c : User processes
 */

#include <xeroskernel.h>
#include <xeroslib.h>
//#include <string.h>



/**
 * PROCESS 1
 */
void proc1(void) {
    /**
     * – Prints a message indicating it is alive.
     * – Sleeps for 5 seconds.
     * – Does a receive from the root process.
     * – Prints a message saying the message has been received and the number
     *   of milliseconds it is to sleep.
     * – Sleeps for the number of milliseconds specified in the received
     *   message.
     * – Prints a message saying sleeping has stopped and it is going to exit.
     * – Runs off the end of its code.
     */
    
    char my_str[1024];
    unsigned long num;
    int parent;
    int my_id;

    my_id = sysgetpid();
    parent = sysgetparent();

    syssleep(5000);

    sysrecv(&parent, &num);

    sprintf(my_str, "PROCESS %d: received from root %d\n", my_id, num);
    sysputs(my_str);
    sprintf(my_str, "PROCESS %d: sleeping for %d\n", my_id, num);
    sysputs(my_str);

    syssleep(num);

    sprintf(my_str, "PROCESS %d: slept well! uhh.. now dying!\n", my_id);
    sysputs(my_str);
}


/**
 * PROCESS 2
 */
void proc2(void) {
    /**
     * – Prints a message indicating it is alive.
     * – Sleeps for 5 seconds.
     * – Does a receive from the root process.
     * – Prints a message saying the message has been received and the number
     *   of milliseconds it is to sleep.
     * – Sleeps for the number of milliseconds specified in the received
     *   message.
     * – Prints a message saying sleeping has stopped and it is going to exit.
     * – Runs off the end of its code.
     */
    
    char my_str[1024];
    unsigned long num;
    int parent;
    int my_id;

    my_id = sysgetpid();
    parent = sysgetparent();

    syssleep(5000);

    sysrecv(&parent, &num);

    sprintf(my_str, "PROCESS %d: received from root %d\n", my_id, num);
    sysputs(my_str);
    sprintf(my_str, "PROCESS %d: sleeping for %d\n", my_id, num);
    sysputs(my_str);

    syssleep(num);

    sprintf(my_str, "PROCESS %d: slept well! uhh.. now dying!\n", my_id);
    sysputs(my_str);
}


/**
 * PROCESS 3
 */
void proc3(void) {
    /**
     * – Prints a message indicating it is alive.
     * – Sleeps for 5 seconds.
     * – Does a receive from the root process.
     * – Prints a message saying the message has been received and the number
     *   of milliseconds it is to sleep.
     * – Sleeps for the number of milliseconds specified in the received
     *   message.
     * – Prints a message saying sleeping has stopped and it is going to exit.
     * – Runs off the end of its code.
     */
    
    char my_str[1024];
    unsigned long num;
    int parent;
    int my_id;

    my_id = sysgetpid();
    parent = sysgetparent();

    syssleep(5000);

    sysrecv(&parent, &num);

    sprintf(my_str, "PROCESS %d: received from root %d\n", my_id, num);
    sysputs(my_str);
    sprintf(my_str, "PROCESS %d: sleeping for %d\n", my_id, num);
    sysputs(my_str);

    syssleep(num);

    sprintf(my_str, "PROCESS %d: slept well! uhh.. now dying!\n", my_id);
    sysputs(my_str);
}


/**
 * PROCESS 4
 */
void proc4(void) {
    /**
     * – Prints a message indicating it is alive.
     * – Sleeps for 5 seconds.
     * – Does a receive from the root process.
     * – Prints a message saying the message has been received and the number
     *   of milliseconds it is to sleep.
     * – Sleeps for the number of milliseconds specified in the received
     *   message.
     * – Prints a message saying sleeping has stopped and it is going to exit.
     * – Runs off the end of its code.
     */
    
    char my_str[1024];
    unsigned long num;
    int parent;
    int my_id;

    my_id = sysgetpid();
    parent = sysgetparent();

    syssleep(5000);

    sysrecv(&parent, &num);

    sprintf(my_str, "PROCESS %d: received from root %d\n", my_id, num);
    sysputs(my_str);
    sprintf(my_str, "PROCESS %d: sleeping for %d\n", my_id, num);
    sysputs(my_str);

    syssleep(num);

    sprintf(my_str, "PROCESS %d: slept well! uhh.. now dying!\n", my_id);
    sysputs(my_str);
}


/**
 * ROOT PROCESS
 */
void root(void) {
    char my_str[1024];
    unsigned long num;
    int my_id;
    int status;
    int proc1_pid;
    int proc2_pid;
    int proc3_pid;
    int proc4_pid;

    my_id = sysgetpid();

    sprintf(my_str, "PROCESS %d: root is alive!\n", my_id);
    sysputs(my_str);

    proc1_pid = syscreate(&proc1, PROC_STACK, my_id);
    proc2_pid = syscreate(&proc2, PROC_STACK, my_id);
    proc3_pid = syscreate(&proc3, PROC_STACK, my_id);
    proc4_pid = syscreate(&proc4, PROC_STACK, my_id);

    sprintf(my_str, "PROCESS %d: created 4 processes: %d, %d, %d, %d\n", my_id, proc1_pid, proc2_pid, proc3_pid, proc4_pid );
    sysputs(my_str);

    syssleep(4000);

    status = syssend(proc1_pid, 10000); // 10 seconds
    sprintf(my_str, "PROCESS %d: sent to %d 10000, status: %d\n", my_id, proc1_pid, status);
    sysputs(my_str);

    status = syssend(proc2_pid, 7000); // 7 seconds
    sprintf(my_str, "PROCESS %d: sent to %d 7000, status: %d\n", my_id, proc2_pid, status);
    sysputs(my_str);

    status = syssend(proc3_pid, 40000); // 20 seconds
    sprintf(my_str, "PROCESS %d: sent to %d 40000, status: %d\n", my_id, proc3_pid, status);
    sysputs(my_str);

    status = syssend(proc4_pid, 27000); // 27 seconds
    sprintf(my_str, "PROCESS %d: sent to %d 27000, status: %d\n", my_id, proc4_pid, status);
    sysputs(my_str);

    //syssleep(50000);

    status = sysrecv(&proc4_pid, &num);
    sprintf(my_str, "PROCESS %d: attempt to receive from %d, status: %d\n", my_id, proc4_pid, status);
    sysputs(my_str);

    status = syssend(proc3_pid, 666);
    sprintf(my_str, "PROCESS %d: attempt to send to %d, status: %d\n", my_id, proc3_pid, status);
    sysputs(my_str);

    sysstop();

    // must not print this!
    sprintf(my_str, "PROCESS %d: DEAD!\n", my_id);
    sysputs(my_str);
}






/**
 * ADDITIONAL TESTS FOR TESTING.TXT
 */

/**
 * COMBINED TESTS SEND AND RECEIVE
 */

void recv1(void) {
    char my_str[1024];
    unsigned long num;
    int result;
    int my_id;
    int parent;

    parent = sysgetparent();
    my_id = sysgetpid();

    sprintf(my_str, "RECV1 %d: Up & running!\n", my_id);
    sysputs(my_str);

    result = sysrecv(&parent, &num);

    sprintf(my_str, "RECV1 %d: attempt to receive from %d, status: %d, result: %d\n", my_id, parent, result, num);
    sysputs(my_str);
}

void recv2(void) {
    char my_str[1024];
    unsigned long num;
    int result;
    int my_id;
    int parent;

    parent = sysgetparent();

    my_id = sysgetpid();

    sprintf(my_str, "RECV2 %d: Up & running!\n", my_id);
    sysputs(my_str);

    result = sysrecv(0, &num);

    sprintf(my_str, "RECV2 %d: attempt to receive from %d, result: %d, value: %d\n", my_id, parent, result, num);
    sysputs(my_str);
}

void send1(void) {
    char my_str[1024];
    int result;
    int my_id;
    int parent;

    syssleep(20000);

    parent = sysgetparent();
    my_id = sysgetpid();
    
    result = syssend(parent, 6666);

    sprintf(my_str, "SEND1 %d: attempt to send to %d, result: %d\n", my_id, parent, result, 6666);
    sysputs(my_str);
}


void send2(void) {
    char my_str[1024];
    int result;
    int my_id;
    int parent;

    syssleep(30000);

    parent = sysgetparent();
    my_id = sysgetpid();
    
    result = syssend(parent, 6666);

    sprintf(my_str, "SEND1 %d: attempt to send to %d, result: %d\n", my_id, parent, result, 6666);
    sysputs(my_str);
}


void root2(void) {
    char my_str[1024];
    unsigned long num;
    int result;
    int my_id;
    int recv1_id;
    int recv2_id;
    int send1_id;
    int send2_id;

    my_id = sysgetpid();

    sprintf(my_str, "ROOT2 %d: Up & running!\n", my_id);
    sysputs(my_str);

    recv1_id = syscreate(&recv1, PROC_STACK, my_id);
    recv2_id = syscreate(&recv2, PROC_STACK, my_id);

    sprintf(my_str, "ROOT2 %d: created 2 receiver processes: %d, %d\n", my_id, recv1_id, recv2_id );
    sysputs(my_str);

    syssleep(40000);

    // sending to recv2
    result = syssend(recv2_id, 123);
    sprintf(my_str, "ROOT2 %d: sent to %d, result: %d\n", my_id, recv2_id, result );
    sysputs(my_str);

    sprintf(my_str, "ROOT2 %d: sleeping...zzz...\n", my_id);
    sysputs(my_str);
    syssleep(10000);

    send1_id = syscreate(&send1, PROC_STACK, my_id);
    send2_id = syscreate(&send2, PROC_STACK, my_id);

    //sending to recv1
    result = syssend(recv1_id, 1234);
    sprintf(my_str, "ROOT2 %d: sent to %d, result: %d\n", my_id, recv1_id, result );
    sysputs(my_str);

    // receiving from sender1
    result = sysrecv(&send1_id, &num);
    sprintf(my_str, "ROOT2 %d: received from %d, result: %d, %d\n", my_id, send1_id, result, send2_id );
    sysputs(my_str);

    sprintf(my_str, "ROOT2 %d: TESTS DONE! Dying.\n", my_id);
    sysputs(my_str);
}







/**
 * SEND FAILURE
 */


void recv3(void) {
    char my_str[1024];
    int my_id;

    my_id = sysgetpid();

    sprintf(my_str, "RECV3 %d: Up & running!\n", my_id);
    sysputs(my_str);

    for (int i = 0; i < 2000; ++i)
    {
        sysyield();
    }

    sprintf(my_str, "RECV3 %d: dying!\n", my_id);
    sysputs(my_str);
}


void send3(void) {
    char my_str[1024];
    int my_id;
    // int parent;

    my_id = sysgetpid();
    sprintf(my_str, "SEND3 %d: Up & running!\n", my_id);
    sysputs(my_str);

    // parent = sysgetparent();
    
    
    // result = syssend(parent, 6666);

    // sprintf(my_str, "SEND3 %d: attempt to send to %d, result: %d\n", my_id, parent, result, num);
    // sysputs(my_str);

    sprintf(my_str, "SEND3 %d: dying!\n", my_id);
    sysputs(my_str);
}


void root3(void) {
    char my_str[1024];
    int result;
    int my_id;
    int recv3_id;
    int send3_id;

    my_id = sysgetpid();

    sprintf(my_str, "ROOT3 %d: Up & running!\n", my_id);
    sysputs(my_str);

    // sending to non-existent process
    result = syssend(100, 123);
    sprintf(my_str, "ROOT3 %d: sent to %d, result: %d\n", my_id, 100, result );
    sysputs(my_str);

    // sending to self
    result = syssend(my_id, 123);
    sprintf(my_str, "ROOT3 %d: sent to %d, result: %d\n", my_id, 100, result );
    sysputs(my_str);

    syssleep(5000);

    // receiver
    recv3_id = syscreate(&recv3, PROC_STACK, my_id);

    // second sender
    send3_id = syscreate(&send3, PROC_STACK, my_id);

    // send to receiver
    result = syssend(recv3_id, 123);
    sprintf(my_str, "ROOT3 %d: sent to %d, result: %d\n", my_id, send3_id, result );
    sysputs(my_str);


    // sending to sender
    result = syssend(send3_id, 123);
    sprintf(my_str, "ROOT3 %d: sent to %d, result: %d\n", my_id, recv3_id, result );
    sysputs(my_str);

    sprintf(my_str, "ROOT3 %d: TESTS DONE! Dying.\n", my_id);
    sysputs(my_str);
}






/**
 * TIME SHARING TESTS
 */

void shared_proc(void) {
    char my_str[1024];
    int my_id;

    my_id = sysgetpid();

    sprintf(my_str, "SHARE %d: Up and running!\n", my_id );
    sysputs(my_str);

    for (;;)
    {
        sprintf(my_str, ".");
        sysputs(my_str);

    }
}

void root4(void) {
    char my_str[1024];
    int my_id;
    int share;

    my_id = sysgetpid();
    sprintf(my_str, "ROOT4 %d: Up and running!\n", my_id );
    sysputs(my_str);

    // second sender
    share = syscreate(&shared_proc, PROC_STACK, my_id);

    sprintf(my_str, "ROOT4 %d: created SHARE: %d\n", my_id, share );
    sysputs(my_str);

    for (;;)
    {
        sprintf(my_str, ":");
        sysputs(my_str);
    }
}








/**
 * RECEIVE FAILURE
 */

void sendDummy(void) {
    // char my_str[1024];
    // unsigned long num;
    // int result;
    // int my_id;
    // int parent;
}


void root5(void) {
    unsigned long i = 0;
    int sendPID = 77;
    int result;
    int pid = sysgetpid();
    int dummyPid = 0;
    
    
    // RECV FAILURE TEST1: receiving from non existent process
    result = sysrecv( &sendPID, &i);

    if(result == -1){
        kprintf("sysrecv in line 558 passed\n");
    } else {
        kprintf("sysrecv in line 558 failed\n");
    }
    

    // RECEIVE FAILURE TEST2: receive to a process that got killed/stopped
    dummyPid = syscreate(&sendDummy, PROC_STACK, pid);
    result = sysrecv( &dummyPid, &i);
    

    if(result == -1){
        kprintf("sysrecv in line 558 passed\n");
    } else {
        kprintf("sysrecv in line 558 failed\n");
    }
    

    // RECEIVE FAILURE TEST3: receive to itself
    result = sysrecv( &pid, &i);
    

    if(result == -2){
        kprintf("sysrecv in line 558 passed\n");
    } else {
        kprintf("sysrecv in line 558 failed\n");
    }
}





/**
 * ASSIGNMENT 3 TESTS!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */



/**
 * testing sysgetcputime() with two functions
 */

// child process for getquantum_master_test()
void getquantum_child_test() {
    int my_id;
    char my_str[512];

    my_id = sysgetpid();
    sprintf(my_str, "getcputime_CHILD: %d: up\n", my_id );
    sysputs(my_str);

    for (int i = 0; i < 100000; ++i);

    sprintf(my_str, "getcputime_CHILD: %d: dying\n", my_id );
    sysputs(my_str);
}

// func instead of root
void getquantum_master_test() {
    int my_id;
    int result;
    int child;
    char my_str[512];

    my_id = sysgetpid();

    child = syscreate(&getquantum_child_test, PROC_STACK, my_id);

    sprintf(my_str, "getcputime_MASTER %d: up. CHILD: up with PID: %d\n",
        my_id, child );
    sysputs(my_str);

    // checking how many quantums my child used so far
    result = sysgetcputime(child);
    sprintf(my_str, "getcputime_MASTER %d: child used %d quantums\n",
        my_id, result);
    sysputs(my_str);

    sysyield();
    sysyield();

    // checking how many quantums a non-exisent process used
    result = sysgetcputime(1000);
    sprintf(my_str, "getcputime_MASTER %d: PID 1000 used %d quantums\n",
        my_id, result);
    sysputs(my_str);
    
    sysyield();
    sysyield();

    // Checking how many quantums I used so far
    result = sysgetcputime(-1);
    sprintf(my_str, "getcputime_MASTER %d: I used %d quantums\n",
        my_id, result);
    sysputs(my_str);

    sysyield();
    sysyield();

    // Checking how many quantums my child used so far
    result = sysgetcputime(child);
    sprintf(my_str, "getcputime_MASTER %d: child used %d quantums\n",
        my_id, result);
    sysputs(my_str);

    sysyield();
    sysyield();

    // checking how many quantums idle process used
    result = sysgetcputime(0);
    sprintf(my_str, "getcputime_MASTER %d: IDLE used %d quantums\n",
        my_id, result);
    sysputs(my_str);

    syssleep(10000);

    // checking how many quantums idle process used
    result = sysgetcputime(0);
    sprintf(my_str, "getcputime_MASTER %d: IDLE used %d quantums\n",
        my_id, result);
    sysputs(my_str);

    sysyield();
    sysyield();

    // Checking how many quantums I used so far
    result = sysgetcputime(-1);
    sprintf(my_str, "getcputime_MASTER %d: I used %d quantums\n",
        my_id, result);
    sysputs(my_str);

    // Print this message and die...
    sprintf(my_str, "getcputime_MASTER %d: Test done, dying...\n",
        my_id, child );
    sysputs(my_str);
}







/**
 * Testing signals!!!
 */

void sig_child(void) {
    int  my_id;
    int  result;
    // int  parent;
    // unsigned long num;
    char my_str[512];
    void (*oldhandler)(void*);

    my_id = sysgetpid();
    sysgetparent();
    sprintf(my_str, "sig_test CHILD: %d: up\n", my_id );
    sysputs(my_str);

    // signal handler 2
    void hndlr2(void* ctx) {
        char my_str[128];
        // kprintf("2\n");
        sprintf(my_str, "RUNNING AN AWESOME HANDLER YUPPYYY!!\n");
        sysputs(my_str);
    }

    // signal handler 3
    void hndlr3(void* ctx) {
        char my_str[128];
        // kprintf("2\n");
        sprintf(my_str, "signal 3 handler\n");
        sysputs(my_str);
    }

    // signal handler 4
    void hndlr4(void* ctx) {
        char my_str[128];
        // kprintf("2\n");
        sprintf(my_str, "signal 4 handler\n");
        sysputs(my_str);
    }

    // signal handler 5
    void hndlr5(void* ctx) {
        char my_str[128];
        // kprintf("2\n");
        sprintf(my_str, "signal 5 handler\n");
        sysputs(my_str);
    }

    // signal handler 6
    void hndlr6(void* ctx) {
        char my_str[128];
        // kprintf("2\n");
        sprintf(my_str, "signal 6 handler\n");
        sysputs(my_str);
    }

    // signal handler 7
    void hndlr7(void* ctx) {
        char my_str[128];
        // kprintf("2\n");
        sprintf(my_str, "signal 7 handler\n");
        sysputs(my_str);
    }

    // register valid signal handler
    result = syssighandler(2, &hndlr2, &oldhandler);
    if (result == 0) {
        sprintf(my_str, "Signal 2 registration: SUCCESS. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Signal 2 registration: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    result = syssighandler(3, &hndlr3, &oldhandler);
    if (result == 0) {
        sprintf(my_str, "Signal 3 registration: SUCCESS. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Signal 3 registration: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    result = syssighandler(4, &hndlr4, &oldhandler);
    if (result == 0) {
        sprintf(my_str, "Signal 4 registration: SUCCESS. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Signal 4 registration: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    result = syssighandler(5, &hndlr5, &oldhandler);
    if (result == 0) {
        sprintf(my_str, "Signal 5 registration: SUCCESS. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Signal 5 registration: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    result = syssighandler(6, &hndlr6, &oldhandler);
    if (result == 0) {
        sprintf(my_str, "Signal 6 registration: SUCCESS. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Signal 6 registration: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    result = syssighandler(7, &hndlr7, &oldhandler);
    if (result == 0) {
        sprintf(my_str, "Signal 7 registration: SUCCESS. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Signal 7 registration: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    //kprintf("Addr hndlr from user.c: 0x%X\n", &hndlr7);

    // register signal with invalid address
    result = syssighandler(7, (void *) 0xFFFFFFFE, &oldhandler);

    if (result == -2) {
        sprintf(my_str, "Invalid handler address: PASSED. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Invalid handler address: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    // register signal with invalid signal number
    result = syssighandler(100, &hndlr7, &oldhandler);

    if (result == -1) {
        sprintf(my_str, "Invalid signal number: PASSED. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "Invalid signal number: FAIL. Result: %d\n",
            result);
        sysputs(my_str);
    }

    // // testing syssleep unblocking
    // result = syssleep(200000);

    // if (result == 0) {
    //     sprintf(my_str, "Sleeping: SUCCESS. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // } else {
    //     sprintf(my_str, "Sleeping: FAIL. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // }

    // // testing recv unblocking
    // result = sysrecv(&parent, &num);

    // if (result == -3) {
    //     sprintf(my_str, "Receive unblocking: PASSED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // } else {
    //     sprintf(my_str, "Receive unblocking: FAIL. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // }

    // pause
    for (int i = 0; i < 100000; ++i);

    sprintf(my_str, "sigtest CHILD: %d: dying\n", my_id );
    sysputs(my_str);
}


void sig_test(void) {
    int my_id;
    int result;
    int child;
    char my_str[512];

    my_id = sysgetpid();


    child = syscreate(&sig_child, PROC_STACK, my_id);

    sprintf(my_str, "sig_test MASTER %d: up. CHILD: up with PID: %d\n",
        my_id, child );
    sysputs(my_str);


    // // signalling self
    // sprintf(my_str, "Signal to self: 9.\n",
    //         result);
    // sysputs(my_str);

    // result = syskill(my_id, 9);
    // if (result == 0) {
    //     sprintf(my_str, "Signal to self: PASSED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // } else {
    //     sprintf(my_str, "Signal to self: FAILED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // }
    
    



    // pause
    for (int i = 0; i < 100000; ++i);

 // send unregistered signal to a process
    result = syskill(child, 25);
    if (result == -651) {
        sprintf(my_str, "UNREGISTERED signal: PASSED. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "UNREGISTERED signal: FAILED. Result: %d\n",
            result);
        sysputs(my_str);
    }

    // send unknown signal
    result = syskill(child, 100);
    if (result == -651) {
        sprintf(my_str, "UNKNOWN signal: PASSED. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "UNKNOWN signal: FAILED. Result: %d\n",
            result);
        sysputs(my_str);
    }

    // send registered legal signal
    result = syskill(child, 2);
    if (result == 0) {
        sprintf(my_str, "DELIVERED signal 2: PASSED. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "DELIVERED signal 2: FAILED. Result: %d\n",
            result);
        sysputs(my_str);
    }

    result = syskill(child, 3);
    if (result == 0) {
        sprintf(my_str, "DELIVERED signal 3: PASSED. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "DELIVERED signal 3: FAILED. Result: %d\n",
            result);
        sysputs(my_str);
    }

    result = syskill(child, 4);
    if (result == 0) {
        sprintf(my_str, "DELIVERED signal 4: PASSED. Result: %d\n",
            result);
        sysputs(my_str);
    } else {
        sprintf(my_str, "DELIVERED signal 4: FAILED. Result: %d\n",
            result);
        sysputs(my_str);
    }

    // result = syskill(child, 5);
    // if (result == 0) {
    //     sprintf(my_str, "DELIVERED signal 5: PASSED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // } else {
    //     sprintf(my_str, "DELIVERED signal 5: FAILED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // }


    // result = syskill(child, 6);
    // if (result == 0) {
    //     sprintf(my_str, "DELIVERED signal 6: PASSED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // } else {
    //     sprintf(my_str, "DELIVERED signal 6: FAILED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // }

    // result = syskill(child, 7);
    // if (result == 0) {
    //     sprintf(my_str, "DELIVERED signal 7: PASSED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // } else {
    //     sprintf(my_str, "DELIVERED signal 7: FAILED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // }

    // // send signal to process that does not exist
    // result = syskill(1000, 7);
    // if (result == -712) {
    //     sprintf(my_str, "PROCESS DNE: PASSED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // } else {
    //     sprintf(my_str, "PROCESS DNE: FAILED. Result: %d\n",
    //         result);
    //     sysputs(my_str);
    // }

    // pause
    for (int i = 0; i < 100000; ++i);

    // Print this message and die...
    sprintf(my_str, "sig_test MASTER %d: Test done, dying...\n",
        my_id, child );
    sysputs(my_str);
}







/**
 * Testing syswait!!!
 */

void wait_child(void) {
    int  my_id;
    int  result;
    int  parent;
    char my_str[512];

    my_id = sysgetpid();
    parent = sysgetparent();
    sprintf(my_str, "sig_wait CHILD: %d: up\n", my_id );
    sysputs(my_str);

    result = syswait(parent);
    
    // die after parent dies
    if (!result) {
        sprintf(my_str, "sig_wait CHILD: parent died! result: %d\n", result );
        sysputs(my_str);
    } else {
        sprintf(my_str, "sig_wait CHILD: wait failed, result: %d\n", result );
        sysputs(my_str);
    }

    sprintf(my_str, "sig_wait CHILD: dying\n");
    sysputs(my_str);
}

void wait_child2(void) {
    int  my_id;
    int  result;
    int  parent;
    // int  child;
    char my_str[512];

    my_id = sysgetpid();
    parent = sysgetparent();

    syscreate(&wait_child, PROC_STACK, my_id);

    sprintf(my_str, "sig_wait CHILD2: %d: up\n", my_id );
    sysputs(my_str);

    result = syswait(parent);
    
    // die after parent dies
    if (!result) {
        sprintf(my_str, "sig_wait CHILD2: parent died! result: %d\n", result );
        sysputs(my_str);
    } else {
        sprintf(my_str, "sig_wait CHILD2: wait failed, result: %d\n", result );
        sysputs(my_str);
    }

    for (int i = 0; i < 10000000; ++i);

    sprintf(my_str, "sig_wait CHILD2: dying\n");
    sysputs(my_str);
}

void wait_test(void) {
    int my_id;
    // int result;
    int child2;
    char my_str[512];

    my_id = sysgetpid();

    child2 = syscreate(&wait_child2, PROC_STACK, my_id);

    sprintf(my_str, "sig_test MASTER %d: up. CHILD: up with PID: %d\n",
        my_id, child2 );
    sysputs(my_str);

    // pause
    for (int i = 0; i < 10000000; ++i);

    sprintf(my_str, "sig_wait MASTER: dying\n");
    sysputs(my_str);
}








void keyboardInitTest(){
    int result;
    char my_str[512];
    int bufflen = 5;
    unsigned char defval = 9;
    unsigned char* buff = &defval;

    // open KBD
    int fd = sysopen(KBD);
    sprintf(my_str, "Keyboard test up. Opened keyboard, FD: %d\n", fd);
    sysputs(my_str);
    sysioctl(fd, KBD_ECHO, 0);

    // opening non-existent device
    int fd2 = sysopen(100);
    sprintf(my_str, "Trying to open device 100, FD: %d\n", fd2);
    sysputs(my_str);

    // unsupported write command
    result = syswrite(fd, (void*) buff, bufflen);
    sprintf(my_str, "Trying to write to keyboard. Result: %d\n", result);
    sysputs(my_str);

    // invalid ioctl command
    result = sysioctl(fd, 100, 0);
    sprintf(my_str, "Trying to send command \"100\" to kbd. Result: %d\n", result);
    sysputs(my_str);

    // pause
    for (int i = 0; i < 15000000; ++i);
    sysread(0, buff, 4);
    sprintf(my_str, "value inside buffer: %c\n", *buff);
    sysputs(my_str);

    
    // enable echoing
    // result = sysioctl(fd, KBD_ECHO, 0);
    // kprintf("IOCTL 1 result: %d\n", result);

    result = sysioctl(fd, 53, 1);
    sprintf(my_str, "Changing EOF symbol. Result: %d\n", result);
    sysputs(my_str);

    // pause
    for (int i = 0; i < 100000; ++i);
    
    // // disable echoing
    // result = sysioctl(fd, KBD_NONECHO, 0);
    
    // for (int i = 0; i < 100000000; ++i);
    
    // // disable keyboard
    // result = sysclose(fd);
    for (int i = 0; i < 100000; ++i);

    // tesing sysclose on unknown device
    result = sysclose(100);
    sprintf(my_str, "Closing unknown device. Result: %d\n", result);
    sysputs(my_str);

    // testing sysclose on open keyboard
    result = sysclose(fd);
    sprintf(my_str, "Closing keyboard. Result: %d\n", result);
    sysputs(my_str);

    // bye bye
    sprintf(my_str, "Dying...\n");
    sysputs(my_str);
}



// SHELL
void shell(void){
    while(1){
        char* command_k  = "k";
        char* command_a  = "a";
        char* command_ps = "ps";
        char* command_m  = "m";
        char* command_t  = "t";
        char* command_ex = "ex";
        unsigned char dummy = 0;
        int my_id = sysgetpid();
        unsigned char* command = &dummy;
        int commandPid;
        char my_str[512];
        // 1. Print the prompt >
        sprintf(my_str, "> \n");
        sysputs(my_str);
        // 2. Reads the command - each command ends when the enter key is pressed.
        sysread(0, command, 4);
        // 3. The first word on the line is the command.
        int commandlen = strlen(command); /* possibly you've saved the length previously */
        command[commandlen - 1] = 0;
        if(strcmp(command, command_k) == 0){
            // 5. If the command exists then create the process corresponding to that command and remember
            // the process ID until the process exits.
            // commandPid = syscreate(&k, PROC_STACK, my_id);
            // 7. Otherwise wait for the command to finish with syswait().
            syswait(commandPid);
            break;
        }
        if(strcmp(command, command_a) == 0){
            // commandPid = syscreate(&a, PROC_STACK, my_id);
            syswait(commandPid);
            break;
        }
        if(strcmp(command, command_ps) == 0){
            // commandPid = syscreate(&ps, PROC_STACK, my_id);
            syswait(commandPid);
            break;
        }
        if(strcmp(command, command_m) == 0){
            // commandPid = syscreate(&m, PROC_STACK, my_id);
            syswait(commandPid);
            break;
        }
        if(strcmp(command, command_t) == 0){
            // commandPid = syscreate(&t, PROC_STACK, my_id);
            syswait(commandPid);
            break;
        }
        if(strcmp(command, command_ex) == 0){
            // commandPid = syscreate(&ex, PROC_STACK, my_id);
            syswait(commandPid);
            break;
        }


        // 4. If the command does not exist print “Command not found” and go to step 1
        sprintf(my_str, "Command Not Found!\n");
        sysputs(my_str);
    }
}

// TESTING
void rootFinal(){
    while(1){
        int my_id = sysgetpid();
        int shellPid;
        int fd;
        char my_str[512];
        unsigned char dummy = 0;
        unsigned char* username = &dummy;
        unsigned char* password = &dummy;
        char* validUsername = "cs415";
        char* validPassword = "EveryoneGetsAnA";
        // 1. Prints a banner that says Welcome to Xeros - an experimental OS
        sprintf(my_str, "Welcome to Xeros - an experimental OS\n");
        sysputs(my_str);
        // 2. Opens the keyboard in no echo mode.
        fd = sysopen(KBD);
        // 3. Turns keyboard echoing on.
        sysioctl(fd, KBD_ECHO, 0);
        // 4. Prints Username:
        sprintf(my_str, "Type your username please: \n");
        sysputs(my_str);
        // 5. Reads the username - the only username you need to support is cs415
        sysread(0, username, 4);
        // 6. Turns keyboard echoing of
        sysioctl(fd, KBD_NONECHO, 0);
        // 7. Prints Password:
        sprintf(my_str, "Type your password please: \n");
        sysputs(my_str);
        // 8. Reads the password - password is EveryoneGetsAnA
        sysread(0, password, 4);
        // 9. Closes the keyboard
        sysclose(fd);
        if((strcmp(username, validUsername) == 0) && (strcmp(password, validPassword) == 0)){
            shellPid = syscreate(&shell, PROC_STACK, my_id);
            syswait(shellPid);
        }
    }
}


void keyboardExtraTest(){
    int fd = sysopen(KBD);
    int bufflen = 5;
    unsigned char defval = 0;
    unsigned char* buff = &defval;
    sysioctl(fd, KBD_ECHO, 0);
    sysioctl(fd, KBD_NONECHO, 0);
    for(int i = 0; i < 10000000; i++);
    sysread(fd, buff, bufflen);
    kprintf("buffer: %c\n", *buff);
    sysioctl(fd, KBD_ECHO, 0);

}








