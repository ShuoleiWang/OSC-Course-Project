#include "osc_queue.h"

#define TIME_SLICE 2
// duration of the time slice for the round robin algorithm

#define PRIORITY 3
// the number of priority levels for priority queue

int generateProcessTime();
// generate the duration a job should be running
long int getDifferenceInMilliSeconds(struct timeval start, struct timeval end);
// calculate the time difference in milliseconds

struct element generateProcess();
// to create an element (i.e. job, or process)

void runProcess(int index, int t);
// simulate the running of jobs

void runNonPreemptiveJob(struct queue *my_arr, int index);
// run the shortest job first algorithm
void runPreemptiveJob(struct queue *my_arr, int index);
// run the round robin for priority queue

//updated for task 3,4,5
void runProcessv2(int pid, int t);
// simulate the running of jobs

void runNonPreemptiveJobv2(struct element * tempProcess);
// run the shortest job first algorithm

void runPreemptiveJobv2(struct element * tempProcess);
// run the round robin for priority queue

