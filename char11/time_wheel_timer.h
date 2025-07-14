#ifndef TIME_WHEEL_TIMER
#define TIME_WHEEL_TIMER

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFER_SIZE 64

class tw_timer;

struct client_data {
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    tw_timer* timer;
};

class tw_timer {
public:
    tw_timer(int rot, int ts) : next(NULL), prev(NULL), rotation(rot), time_slot(ts) {}

public:
    int rotation;
    int time_slot;
    void (*cb_func) (client_data*);
    client_data* user_data;
    tw_timer* next;
    tw_timer* prev;
};

class time_wheel {
public:
    time_wheel() : cur_slot(0) {
        for(int i = 0; i < N; ++i) {
            slot[i] = NULL;
        }
    }
    ~time_wheel() {
        for(int i = 0; i < N; ++i) {
            tw_timer* tmp = slot[i];
            while(tmp) {
                slots[i] = tmp->next;
                delete tmp;
                tmp = slots[i];
            }
        }
    }

    tw_timer* add_timer(int timeout) {
        if (timeout < 0) {
            return NULL;
        }
        int ticks = 0;
        if (timeout < SI) {
            ticks = 1;
        }
        else {
            ticks = timeout / SI;
        }
        int rotation = ticks / N;
        int ts = (cur_slot + (ticks % N)) %N;
        tw_timer* timer = new tw_timer(rotation, ts);
        if (!slots[ts]) {
            printf("add timer, rotation is %d, ts is %d, cur_slot is %d\n", rotation, ts, cur_slot);
            slots[ts] = timer;
        }
        else {
            timer->next = slots[ts];
            slots[ts]->prev = timer;
            slots[ts] = timer;
        }
        return timer;
    }
    
    void del_timer(tw_timer* timer) {
        if (!timer) {
            return;
        }
        int ts = timer->time_slot;
        if (timer == slots[ts]) {
            slots[ts] = slots[ts]->next;
            if (slots[ts]) {
                slots[ts]->prev = NULL;
            }
        }
    }
}