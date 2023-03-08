#include <limits.h>
#include "oslabs.h"

struct PCB NULLPCB = {
    .process_id = 0,
    .arrival_timestamp = 0,
    .total_bursttime = 0,
    .execution_starttime = 0,
    .execution_endtime = 0,
    .remaining_bursttime = 0,
    .process_priority = 0
};

int is_null_pcb(struct PCB block) {
    if (block.process_id == 0 && 
        block.arrival_timestamp == 0 &&
        block.total_bursttime == 0 &&
        block.execution_starttime == 0 &&
        block.execution_endtime == 0 &&
        block.remaining_bursttime == 0 &&
        block.process_priority == 0
    ) { return 1; }
    return 0;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (is_null_pcb(current_process)) { 
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process; 
    }

    if (current_process.process_priority <= new_process.process_priority) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[(*queue_cnt)++] = new_process;
        return current_process;
    }

    current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
    current_process.execution_endtime = 0;
    ready_queue[(*queue_cnt)++] = current_process;
    new_process.execution_starttime = timestamp;
    new_process.execution_endtime = timestamp + new_process.total_bursttime;
    new_process.remaining_bursttime = new_process.total_bursttime;
    return new_process;
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (!*queue_cnt) { return NULLPCB; }

    // search the next PCB
    int highestPriorty = INT_MAX, priorityIdx = 0;
    for (int i = 0; i < *queue_cnt; i++) {
        if (ready_queue[i].process_priority < highestPriorty) { 
            highestPriorty = ready_queue[i].process_priority; 
            priorityIdx = i;
        }
    }
    struct PCB nextPCB = ready_queue[priorityIdx];
    nextPCB.execution_starttime = timestamp;
    nextPCB.execution_endtime = timestamp + nextPCB.remaining_bursttime;

    // shift queue elems & update queue counter
    (*queue_cnt)--;
    for (int i = priorityIdx; i < *queue_cnt; i++) {
        ready_queue[i] = ready_queue[i+1];
    }

    return nextPCB;
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
    if (is_null_pcb(current_process)) {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    if(current_process.remaining_bursttime <= new_process.remaining_bursttime) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[(*queue_cnt)++] = new_process;
        return current_process;
    }

    current_process.remaining_bursttime = current_process.execution_endtime - time_stamp;
    current_process.execution_starttime = 0;
    current_process.execution_endtime = 0;
    ready_queue[(*queue_cnt)++] = current_process;
    new_process.execution_starttime = time_stamp;
    new_process.execution_endtime = time_stamp + new_process.total_bursttime;
    new_process.remaining_bursttime = new_process.total_bursttime;
    return new_process;
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (!*queue_cnt) { return NULLPCB; }

    // search the next PCB
    int smallestBurst = INT_MAX, burstIdx = 0;
    for (int i = 0; i < *queue_cnt; i++) {
        if (ready_queue[i].remaining_bursttime < smallestBurst) { 
            smallestBurst = ready_queue[i].remaining_bursttime;
            burstIdx = i;
        }
    }
    struct PCB nextPCB = ready_queue[burstIdx];
    nextPCB.execution_starttime = timestamp;
    nextPCB.execution_endtime = timestamp + nextPCB.remaining_bursttime;

    // shift queue elems & update queue counter
    (*queue_cnt)--;
    for (int i = burstIdx; i < *queue_cnt; i++) {
        ready_queue[i] = ready_queue[i+1];
    }

    return nextPCB;
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB 
    new_process, int timestamp, int time_quantum) {

    if (is_null_pcb(current_process)) { 
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + MIN(new_process.total_bursttime, time_quantum);
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process; 
    }

    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    ready_queue[(*queue_cnt)++] = new_process;
    return current_process;
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum) {
    if (!*queue_cnt) { return NULLPCB; }

    // search the next PCB
    int earliestTime = INT_MAX, timeIdx = 0;
    for (int i = 0; i < *queue_cnt; i++) {
        if (ready_queue[i].arrival_timestamp < earliestTime) { 
            earliestTime = ready_queue[i].arrival_timestamp;
            timeIdx = i;
        }
    }
    struct PCB nextPCB = ready_queue[timeIdx];
    nextPCB.execution_starttime = time_stamp;
    nextPCB.execution_endtime = time_stamp + MIN(nextPCB.remaining_bursttime, time_quantum);

    // shift queue elems & update queue counter
    (*queue_cnt)--;
    for (int i = timeIdx; i < *queue_cnt; i++) {
        ready_queue[i] = ready_queue[i+1];
    }

    return nextPCB;
}