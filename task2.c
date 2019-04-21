#include <stdio.h>
#include "coursework.h"
#include <sys/time.h>

#define MAX_PROCESSES 5

void generateSJF(struct queue* q);
void runSJF(struct queue *q);

struct element e[MAX_PROCESSES];

int main()
{
	struct queue q;

	init(&q, MAX_PROCESSES);
	
	printAll(&q);
	

	printf("Generating processes for SJF ...\n");
	
	
	generateSJF(&q);

	printf("Running the processes using SJF ...\n");

	runSJF(&q);

	freeAll(&q);
	return 0;
}


void generateSJF(struct queue* q)
{
	for(int i=0; i<MAX_PROCESSES; i++)
	{
		if(i == 0)
		{
			printf("SJF: add first\n");
			
		}
		else
		{
			printf("SJF: add new largest\n");
			
		}
		e[i] = generateProcess();
		int index = 0;

		for(int j=0; j<i; j++)
		{			
			if(e[i].pid_time < e[j].pid_time)////>
			{
				index++;
			}
		}
		addHere(q, &e[i], index);
		printAll(q);
		
	}
	
}


void runSJF(struct queue* q)
{
	int AverRes[MAX_PROCESSES];
	int AverTA[MAX_PROCESSES];		
	for(int i=MAX_PROCESSES-1; i>=0; i--)
	{
		struct timeval start, end;
		gettimeofday(&start, NULL);

		runNonPreemptiveJob(q, i);

		
		gettimeofday(&end, NULL);

		printf("C:  %d S:  %d E:  %d R:  %ld T:  %ld\n", q->e[i].created_time.tv_sec, start.tv_sec, end.tv_sec, getDifferenceInMilliSeconds(q->e[i].created_time, start), getDifferenceInMilliSeconds(q->e[i].created_time, end));
		

		AverRes[i] = (int) getDifferenceInMilliSeconds(q->e[i].created_time, start);
		AverTA[i] = (int) getDifferenceInMilliSeconds(q->e[i].created_time, end);
	}

	double AverR, AverT; 
	for(int i=0; i<MAX_PROCESSES; i++)
	{
		AverR+=AverRes[i];
		AverT+=AverTA[i];	
	}
	printf("\nAverage response time: %.2f milliseconds\n", AverR/MAX_PROCESSES);
	printf("Average turn around time: %.2f milliseconds\n", AverT/MAX_PROCESSES);
	

}
	
	
