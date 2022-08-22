#ifndef STATS_H_
#define STATS_H_

typedef struct stats {
    struct timeval arrival_time;
    int dynamic_req;
    int static_req;
    int total_req;
    int thread_id;
    struct timeval wait_time;
}Stats;

#endif