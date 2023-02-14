#include "oslabs.h"

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int
*queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {

}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int
*queue_cnt, int timestamp) {

}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int
*queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {

}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int
*queue_cnt, int timestamp) {

}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int
*queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int
time_quantum) {

}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int
*queue_cnt, int time_stamp, int time_quantum) {
    
}


   struct PCB {
        int process_id;
        int arrival_timestamp;
        int total_bursttime;
        int execution_starttime;
        int execution_endtime;
        int remaining_bursttime;
        int process_priority;
    }

// Each process in an operating system is managed by using a data structure called the Process Control Block (PCB).

// The set of processes in the operating system that are ready to execute are maintained in a data structure
// called the Ready Queue. 

// The NULLPCB is defined as [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0]

// To determine the schedule of execution for the processes in an operating system, we consider three policies:

// Priority-based Preemptive Scheduling (PP)
// Shortest-Remaining-Time-Next Preemptive Scheduling (SRTP)
// Round-Robin Scheduling (RR)
