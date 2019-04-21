#include<semaphore.h>  
#include<pthread.h>
#include "coursework.h"

#define MAX_NUMBER_OF_JOBS 50
#define MAX_BUFFER_SIZE 10


sem_t empty[PRIORITY], full;

//struct queue q[PRIORITY];
struct queue *q;
double AverR, AverT;
 
int p = 0;
int c = 0;

int buffer_count[PRIORITY];       //represent the products in buffer
int value[PRIORITY];

int flag = 0;
int s = PRIORITY * MAX_BUFFER_SIZE;

pthread_mutex_t mutex;

void *producer(void *arg);
void *consumer(void *arg);

int main(void)
{
	q = (struct queue*)malloc(PRIORITY * sizeof(struct queue));
	          
	for(int i=0; i<PRIORITY; i++)
	{
		init(&q[i], MAX_BUFFER_SIZE);
		printf("Q #%d\n", i);
		printAll(&q[i]);

		sem_init (&empty[i], 0, PRIORITY*MAX_BUFFER_SIZE); //initializa "empty" semaphore

		value[i] = i;
	}

	sem_init (&full, 0, 0);  
	pthread_t thrd_prod, thrd_cons1, thrd_cons2, thrd_cons3; 

	//create thread
	if(pthread_create(&thrd_cons3, NULL, consumer, &value[2]) != 0 )  
        	printf("thread create failed.\n");
	if(pthread_create(&thrd_prod, NULL, producer, NULL) != 0 )  
        	printf("thread create failed.\n");
    	if(pthread_create(&thrd_cons1, NULL, consumer, &value[0]) != 0 )  
        	printf("thread create failed.\n");
    	if(pthread_create(&thrd_cons2, NULL, consumer, &value[1]) != 0 )  
        	printf("thread create failed.\n");
    	

 	if(pthread_join(thrd_cons3, NULL) != 0)  
       		printf("wait thread failed.");
    	//waiting for thread finish  
    	if(pthread_join(thrd_prod, NULL) != 0)  
        	printf("wait thread failed.");  
    	if(pthread_join(thrd_cons1, NULL) != 0)  
       		printf("wait thread failed.");
    	if(pthread_join(thrd_cons2, NULL) != 0)  
       		printf("wait thread failed.");
   

          
	//free semaphore
    	sem_destroy(&full);             

	for(int i=0; i<PRIORITY; i++)
	{
		sem_destroy(&empty[i]); 
		freeAll(&q[i]);
	}

	free(q);
	q = NULL;

	return 0;
}

//*/

void *producer(void *arg)
{
	while(p < MAX_NUMBER_OF_JOBS){

        struct element e = generateProcess();
		
		flag++;

		sem_wait(&empty[e.pid_priority]);

		if (addFirst(&q[e.pid_priority], &e) == 0)
		{ 
			buffer_count[e.pid_priority]++;   //buffer++
		}
		else
		{
			exit(0);          //couldn't addFirst
		}

        	p++;
		//printAll(&q[e.pid_priority]);
		printf("P: job priority is %d, job on buffer %d, job produced %d, job consumed %d\n", e.pid_priority, buffer_count[e.pid_priority], p, c);


   		sem_post(&full);
	}

}

void *consumer(void *arg)
{

	
        while(c < MAX_NUMBER_OF_JOBS)
	{
        	int f = 0;

		sem_wait(&full);

		int value = *(int*)arg;
		value --;   ////////////////// why the value +1 in the program!!!
		//printAll(&q[e.pid_priority]);
		while (q[value].count > 0)
		{
			struct element e = q[value].e[q[value].count-1];
			
			int counter = q[value].count-1;

			if (q[value].count > 0)
			{
				//printAll(&q[e.pid_priority]); ////before remove
				removeLast(&q[value]);
				//printAll(&q[e.pid_priority]);
				buffer_count[e.pid_priority]--;
			}
			else
			{
				exit(0);
			}

			struct timeval start, end;

			if (f <= counter)
			{
				gettimeofday(&start, NULL);
				AverR += getDifferenceInMilliSeconds(e.created_time, start);
			}

			if (e.pid_time <= TIME_SLICE)	//if can consume the element totally
			{
				//Running the job
				runPreemptiveJobv2(&e);

				gettimeofday(&end, NULL);  
				AverT += getDifferenceInMilliSeconds(e.created_time, end);   //the average time

				c++;

				sem_post(&empty[value]);
			} 
			else
			{ 
			//Running the job
			runPreemptiveJobv2(&e);
			}
			
			if (e.pid_time > 0){			
				addFirst(&q[value], &e);
				//printAll(&q[e.pid_priority]);
				buffer_count[e.pid_priority]++;
			}

			f++; // this is use for computer the response time

			printf("C: job priority is %d, the consumer number is %d, job on buffer %d, job produced %d, job consumed %d\n", e.pid_priority, *(int*)arg-1, buffer_count[e.pid_priority], p, c); 	
		}

		if (p == MAX_NUMBER_OF_JOBS)      //!!!!!!!!!!must use this to make sure the threads will terminate!!!
		{
			
			s--;
	  		return arg;
		}
	}
}

