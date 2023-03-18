#include "oslabs.h"

int _handle_page_in_memory(struct PTE page_table[TABLEMAX], int page_number, int current_timestamp) {
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count++;
    return page_table[page_number].frame_number;
}

int _handle_free_frame(struct PTE page_table[TABLEMAX], int page_number, int current_timestamp, int frame_pool[POOLMAX], int *frame_cnt) {
    page_table[page_number].is_valid = 1;
    page_table[page_number].frame_number = frame_pool[--(*frame_cnt)];
    page_table[page_number].arrival_timestamp = current_timestamp; 
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;
    return page_table[page_number].frame_number;
}

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {
    if (page_table[page_number].is_valid) {
        return _handle_page_in_memory(page_table, page_number, current_timestamp);
    }
    
    if (*frame_cnt > 0) {
        return _handle_free_frame(page_table, page_number, current_timestamp, frame_pool, frame_cnt);
    }
    
    int lowest_AT_index = -1;
    for (int i=0; i<*table_cnt; i++) {
        if (page_table[i].is_valid) {
            if (lowest_AT_index < 0) { lowest_AT_index = i; }
            else if (page_table[i].arrival_timestamp < page_table[lowest_AT_index].arrival_timestamp) { lowest_AT_index = i; }
        }
    }
    if (lowest_AT_index >= 0) {
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = page_table[lowest_AT_index].frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp; 
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[lowest_AT_index].is_valid = 0;
        page_table[lowest_AT_index].frame_number = -1;
        page_table[lowest_AT_index].arrival_timestamp = -1;
        page_table[lowest_AT_index].last_access_timestamp = -1;
        page_table[lowest_AT_index].reference_count = -1;
        return page_table[page_number].frame_number;
    }
    return -1;
} 

int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX],int frame_cnt) {
    int current_timestamp = 1;
    int total_fault = 0;
    for (int i=0; i<reference_cnt; i++) {
        current_timestamp++;
        int page_number = refrence_string[i];
        if (!page_table[page_number].is_valid) { total_fault++; }
        process_page_access_fifo(page_table, &table_cnt, page_number, frame_pool, &frame_cnt, current_timestamp);
    }
    return total_fault;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
    return 0;
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX],int frame_cnt) {
    return 0;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {
    return 0;
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    return 0;
}
