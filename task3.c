#include <stdio.h>
#include "coursework.h"

#define MAX_PROCESSES 5

void runPQ(struct queue *que);
void generatePQ(struct queue *q);

struct queue *q ;
double AverR = 0; 
double AverT = 0; 

int main()
{
	//struct queue *q = (struct queue *)malloc((PRIORITY) * sizeof(struct queue));
	q = (struct queue *)malloc((PRIORITY) * sizeof(struct queue));
	generatePQ(q);

	printf("\nRunning the processes using PQ ...\n\n");

	runPQ(q);

	printf("Average response time: %.2f milliseconds\n", AverR/MAX_PROCESSES);
	printf("Average turn around time: %.2f milliseconds\n", AverT/MAX_PROCESSES);

	freeAll(q);
	free(q);
	q = NULL;
	return 0;
}


void generatePQ(struct queue *q)
{
	for(int i=0; i<PRIORITY; i++)
	{
		init(&q[i], MAX_PROCESSES);
		printf("Q #%d\n", i);;
		printAll(&q[i]);
	}
		
	printf("Generating processes for PQ ...\n");
	
	struct element e[MAX_PROCESSES];
	
	for(int i=0; i<MAX_PROCESSES; i++)
	{
		e[i] = generateProcess();
		printf("New process has priority %d\n", e[i].pid_priority);
		printf("PQ %d: add now\n", e[i].pid_priority);
		addFirst(&q[e[i].pid_priority], &e[i]);

		for(int j=0; j<PRIORITY; j++)
		{
			printf("Q #%d\n", j);;
			printAll(&q[j]);
		}
	}
}




void runPQ(struct queue *que)//[PRIORITY]
{
	for(int i=0; i<PRIORITY; i++)
	{
		int x = 0;		//this is used to label the response time sentence
		int index = 0;		//this is used to label the turn_around time sentence

		while(que[i].e[0].pid_time > 0)
		{
			struct element temp = que[i].e[que[i].count-1]; //temp is the element in the end of queue
			int counter = que[i].count;
			//printAll(&que[i]);
			removeLast(&que[i]); // remover the element in the last of queue
			
			if(temp.pid_time<=TIME_SLICE){
				index=1;
			}
			struct timeval start, end;
			if(x<counter)
			{	
				gettimeofday(&start, NULL);//start
				printf("Q:  %d P:  %d C:  %d S:  %d R:  %d\n", i, temp.pid, temp.created_time.tv_sec, start.tv_sec, getDifferenceInMilliSeconds(temp.created_time, start));
				AverR += getDifferenceInMilliSeconds(temp.created_time, start);
			}
			printf("The index is %d\n", counter-1);
			runPreemptiveJobv2(&temp); 

			if(index==1){
				gettimeofday(&end, NULL);  //end
				printf("Q:  %d P:  %d C:  %d E:  %d T:  %d\n\n", i, temp.pid, temp.created_time.tv_sec, end.tv_sec, getDifferenceInMilliSeconds(temp.created_time, end));
				AverT += getDifferenceInMilliSeconds(temp.created_time, end);
				//index--;
			}

			if(temp.pid_time!=0)
				addFirst(&que[i], &temp);
			x++;
			index--;
		}
	}

}




