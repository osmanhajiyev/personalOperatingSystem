/* msg.c : messaging system 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>

/* Your code goes here */


// Puts the given source pcb into the end of queue of Senders of dest pcb
void enQueue( pcb* source, pcb* dest ){
    pcb* temp = dest->queueSenders;
    if(!temp){
        dest->queueSenders = source;
    } else {
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = source;
        source->next = NULL;
    }


    //kprintf("queueSenders of %d: ", dest->pid);
    // temp = dest->queueSenders;
    // while(temp != NULL){
    //     kprintf("%d -> ", temp->pid);
    //     temp = temp->next;
    // }
    // kprintf("\n");
}

// Removes given element from queues of source pcb, updates the queue after removal
int removeFromQueue( int elemToRemove, pcb* source ){
    int result = 0;
    pcb *prev, *temp;
    temp = source->queueSenders;
    prev = source->queueSenders;
    if( elemToRemove != NULL && temp != NULL ){
        temp = temp->next;
        if(prev->pid == elemToRemove){
            source->queueSenders = source->queueSenders->next;
            prev->next = NULL;
            result = 1;
        } else {
            // look for the elemenent in the queue of senders of source
            while(temp != NULL){
                if( temp->pid == elemToRemove ){
                    prev->next = temp->next;
                    result = 1;
                    break;
                }
                temp = temp->next;
                prev = prev->next;
            }
        }
    }
    return result;
}

// the send locks until the operation completes
// returns 0 on successful send operation
// returns -1 no proc no longer exists
// if sending to itself then return -2
// -4 if blocked
// for other errors returns -3
int send( int dest_pid, unsigned long num, pcb* source ){ 
    // put given params into given proc's pcb
    // if (no receive waiting) then block
    // if (recv waiting) then copy message from recv in blocked
    // queue to recv buffer and place both procs on ready queue
    // see if process exists in pcbtable if not return -1
    int returnVal = -3;
    unsigned long* tempBuffer;
    int i = 1;
    // CASE: SENDING TO ITSELF
    if(dest_pid == source->pid){
        returnVal = -2;
        source->ret = returnVal;
    } else {
        // LOOK FOR DESIRED PCB IN PROCESS TABLE
        for( i = 0; i < MAX_PROC; i++){
            if(dest_pid == proctab[i].pid && proctab[i].state != STATE_STOPPED){
                //kprintf("process found pid: %d\n", proctab[i].pid);
                break;
            }
            if(i+1 == MAX_PROC){
                returnVal = -1;
                source->ret = returnVal;
                //kprintf("process to send to not found \n");
                break;
            }
        } 
        if(returnVal != -1){
            // IF DESIRED PROCESS IS WAITING FOR US THEN DO SEND OPERATION
            if (proctab[i].state == STATE_BLOCKED_RECV && (proctab[i].waitingPID == source->pid || proctab[i].waitingPID == RECV_ANY)){ 
                // IF FOUND: PUT A MESSAGE IN RECEIVER BUFFER
                //kprintf("send operation happened \n");
                // Set Buffers of send and recv proc's
                proctab[i].buffer = num;
                source->buffer = num;
                tempBuffer = proctab[i].bufferPointer;
                *tempBuffer = num;
                // Set return values of send and recv proc's
                proctab[i].ret = 0;
                source->ret = 0;
                returnVal = 0;
                // Put receiver process in readyQueue
                proctab[i].waitingPID = 0;
                proctab[i].next = NULL;
                ready(&proctab[i]);
            } else {
                // put the current process in blocked queue
                source->buffer = num;
                source->state = STATE_BLOCKED_SEND;
                source->waitingPID = dest_pid;
                source->next = NULL;
                enQueue( source, &proctab[i] );
                returnVal = -4;
                //kprintf("send blocked \n");
            } 
        }
    }
    //kprintf("send reached the end, returnVal = %d\n", returnVal);
    return returnVal;
}



// the recv locks until the operation completes
// PID is 0, then receive from any process
// returns 0 if message received
// -1 PID to receive from is invalid
// -2 if receive to itself
// -4 if blocked
// -3 for all other errors (invalid buffer address)
int recv( int *from_pid, unsigned long *num, pcb* source){
    // if PID = 0, then recv from earliest send
    // if (no send waiting) then block
    // if (send waiting) then copy message from
    // send in blocked queue and put both in ready queue
    int returnVal = -3;
    int i = 0;
    pcb* temp;
    // IF RECEIVING FROM ITSELF
    if(*from_pid == source->pid){
        returnVal = -2;
        source->ret = returnVal;
    } else {


        // Receive Any
        if(*from_pid == 0){
            //kprintf("Entered receiveAny \n");
            if(source->queueSenders != NULL){
                // take the first elem in senderQueue
                //kprintf("Preparing for receiveAny operation \n");
                temp = source->queueSenders; 
                // update senderQueue
                source->queueSenders = source->queueSenders->next; 
                // Set Buffers of send and recv proc's
                source->buffer = temp->buffer;
                *num = temp->buffer;
                // Set return values of send and recv proc's
                temp->ret = 0;
                source->ret = 0;
                returnVal = 0;
                temp->waitingPID = 0;
                temp->next = NULL;
                // Put sender process in readyQueue
                ready(temp);
                //kprintf("Exited receiveAny operation \n");

                //kprintf("queueSenders of %d: ", source->pid);
                // temp = source->queueSenders;
                // while(temp != NULL){
                //     kprintf("%d -> ", temp->pid);
                //     temp = temp->next;
                // }
                // kprintf("\n");

            } else {
                // put the current process in blocked queue
                source->state = STATE_BLOCKED_RECV;
                source->waitingPID = RECV_ANY; 
                source->bufferPointer = num;
                source->next = NULL;
                returnVal = -4;
                //kprintf("recvAny blocked \n");
            }



        } else {

            // Normal receive

            // Search for the destination process to receive from
            for( i = 1; i < MAX_PROC; i++){
                if(*from_pid == proctab[i].pid && proctab[i].state != STATE_STOPPED){
                    break;
                }
                if(i+1 == MAX_PROC){
                    returnVal = -1;
                    source->ret = returnVal;
                    //kprintf("process to recv from not found \n");
                    break;
                }
            }


            if(returnVal != -1){
                // IF DESIRED PROCESS IS WAITING FOR US THEN DO RECEIVE OPERATION
                if (proctab[i].state == STATE_BLOCKED_SEND &&
                    proctab[i].waitingPID == source->pid) {
                    // perform rcv operation
                    // put them both in ready queue
                    //kprintf("recv operation happened \n");
                    // Set Buffers of send and recv proc's
                    source->buffer = proctab[i].buffer;
                    *num = proctab[i].buffer;
                    // Set return values of send and recv proc's
                    proctab[i].ret = 0;
                    source->ret = 0;
                    returnVal = 0;
                    // CHECK IF NOT IN QUEUE OF SENDERS of the current process
                    removeFromQueue( *from_pid, source);
                    // Put sender process in readyQueue
                    proctab[i].waitingPID = 0;
                    proctab[i].next = NULL;
                    ready(&proctab[i]);
                } else {
                    // put the current process in blocked queue
                    source->state = STATE_BLOCKED_RECV;
                    source->waitingPID = *from_pid;
                    source->bufferPointer = num;
                    source->next = NULL;
                    returnVal = -4;
                    //kprintf("recv blocked %d\n", source->pid);
                }
            }
    }

    }
    return returnVal;
}


