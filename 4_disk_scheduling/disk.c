#include "oslabs.h"

struct RCB NULLRCB = {
        .request_id = 0,
        .arrival_timestamp = 0,
        .cylinder = 0,
        .address = 0,
        .process_id = 0
};

int _is_null_RCB(struct RCB block) {
    if (block.request_id == 0 &&
        block.arrival_timestamp == 0 &&
        block.cylinder == 0 &&
        block.address == 0 &&
        block.process_id == 0
    ) { return 1; }
    return 0;
}

void _remove_from_RCB_queue(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int target_index) {
    for(int i=target_index; i<*queue_cnt; i++) {
        request_queue[i] = request_queue[i+1];
    }
    (*queue_cnt)--;
}

int _get_distance(int first_cylindar, int second_cylindar) {
    int dist = first_cylindar - second_cylindar;
    if (dist < 0) {return -dist; }
    return dist;
}

struct RCB _handle_request_arrival(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if(_is_null_RCB(current_request)) { return new_request; }
    request_queue[(*queue_cnt)++] = new_request;
    return current_request;
}

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    return _handle_request_arrival(request_queue, queue_cnt, current_request, new_request, timestamp);
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt) {
    if (*queue_cnt == 0) { return NULLRCB; }
    int earliest_arrival_index = 0;
    for (int i=1; i<*queue_cnt; i++) {
        if (request_queue[i].arrival_timestamp < request_queue[earliest_arrival_index].arrival_timestamp) {
            earliest_arrival_index = i;
        }
    }
    struct RCB next_RCB = request_queue[earliest_arrival_index];
    _remove_from_RCB_queue(request_queue, queue_cnt, earliest_arrival_index);
    return next_RCB;
} 

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    return _handle_request_arrival(request_queue, queue_cnt, current_request, new_request, timestamp);
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder) {
    if (*queue_cnt == 0) { return NULLRCB; }
    int closest_cylinder_index = 0;
    for (int i=1; i<*queue_cnt; i++) {
        if (_get_distance(request_queue[i].cylinder, current_cylinder) < _get_distance(request_queue[closest_cylinder_index].cylinder, current_cylinder)) {
            closest_cylinder_index = i;
        }
        if (_get_distance(request_queue[i].cylinder, current_cylinder) == _get_distance(request_queue[closest_cylinder_index].cylinder, current_cylinder)) {
            closest_cylinder_index = request_queue[i].arrival_timestamp < request_queue[closest_cylinder_index].arrival_timestamp ? i : closest_cylinder_index;
        }
    }
    struct RCB next_RCB = request_queue[closest_cylinder_index];
    _remove_from_RCB_queue(request_queue, queue_cnt, closest_cylinder_index);
    return next_RCB;
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    return _handle_request_arrival(request_queue, queue_cnt, current_request, new_request, timestamp);
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction) {
    if (*queue_cnt == 0) { return NULLRCB; }
    int next_RCB_index = -1;
    int have_matching_cylinder = 0;
    for (int i=0; i<*queue_cnt; i++) {
        if (have_matching_cylinder || request_queue[i].cylinder == current_cylinder) {
            if (have_matching_cylinder) {
                next_RCB_index = request_queue[i].arrival_timestamp < request_queue[next_RCB_index].arrival_timestamp ? i : next_RCB_index;
            }
            else {
                have_matching_cylinder = 1;
                next_RCB_index = i;
            }
            continue;
        }
        int RCB_meets_condition = 0;
        switch (scan_direction){
            case 1:
                if(request_queue[i].cylinder > current_cylinder) { RCB_meets_condition = 1; }
                break;

            case 0:
                if(request_queue[i].cylinder < current_cylinder) { RCB_meets_condition = 1; }
                break;
        }
        if (RCB_meets_condition) {
            if (next_RCB_index < 0) { next_RCB_index = i; }
            else {
                next_RCB_index = _get_distance(request_queue[i].cylinder, current_cylinder) < _get_distance(request_queue[next_RCB_index].cylinder, current_cylinder) ?
                    i : next_RCB_index;
            }
        }
    }
    if (next_RCB_index < 0) { return handle_request_completion_sstf(request_queue, queue_cnt, current_cylinder); }

    struct RCB next_RCB = request_queue[next_RCB_index];
    _remove_from_RCB_queue(request_queue, queue_cnt, next_RCB_index);
    return next_RCB;
}