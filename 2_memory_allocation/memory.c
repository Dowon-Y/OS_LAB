#include "oslabs.h"

struct MEMORY_BLOCK NULLBLOCK = {
    .start_address = 0,
    .end_address = 0,
    .segment_size = 0,
    .process_id = 0
};

void shift_memory(struct MEMORY_BLOCK memory_map[MAPMAX], int target_index, int *map_cnt) {
    for (int i = *map_cnt; i > target_index; i--) {
        memory_map[i] = memory_map[i-1];
    }
    (*map_cnt)++;
}

void allocate_memory(struct MEMORY_BLOCK memory_map[MAPMAX], int index, int request_size, int process_id) {
    memory_map[index].end_address = memory_map[index].start_address + request_size - 1;
    memory_map[index].segment_size = request_size;
    memory_map[index].process_id = process_id;
    memory_map[index+1].start_address = memory_map[index].end_address + 1;
    memory_map[index+1].segment_size -= request_size;
    return memory_map[index];
}

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    if (request_size <= 0) { return NULLBLOCK; }

    int best_index = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0) {
            if (memory_map[i].segment_size == request_size) {
                memory_map[i].process_id = process_id;
                return memory_map[i];
            }
            if (memory_map[i].segment_size > request_size) {
                if (best_index < 0) {
                    best_index = i;
                }
                else {
                    if (memory_map[best_index].segment_size > memory_map[i].segment_size) { best_index = i; }
                }
            }
        }
    }
    
    if (best_index >= 0) {
        shift_memory(memory_map, best_index, map_cnt);
        allocate_memory(memory_map, best_index, request_size, process_id);
        return memory_map[best_index];
    }

    return NULLBLOCK;
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    if (request_size <= 0) { return NULLBLOCK; }

    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0) {
            if(memory_map[i].segment_size == request_size) {
                memory_map[i].process_id = process_id;
                return memory_map[i];
            }
            if(memory_map[i].segment_size > request_size) {
                shift_memory(memory_map, i, map_cnt);
                allocate_memory(memory_map, i, request_size, process_id);
                return memory_map[i];
            }
        }
    }

    return NULLBLOCK;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    if (request_size <= 0) { return NULLBLOCK; }

    int worst_index = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0) {
            if (memory_map[i].segment_size == request_size) {
                memory_map[i].process_id = process_id;
                return memory_map[i];
            }
            if (memory_map[i].segment_size > request_size) {
                if (worst_index < 0) {
                    worst_index = i;
                }
                else {
                    if (memory_map[worst_index].segment_size < memory_map[i].segment_size) { worst_index = i; }
                }
            }
        }
    }
    
    if (worst_index >= 0) {
        shift_memory(memory_map, worst_index, map_cnt);
        allocate_memory(memory_map, worst_index, request_size, process_id);
        return memory_map[worst_index];
    }

    return NULLBLOCK;
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address) {
    return NULLBLOCK;
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt) {
    return;
}