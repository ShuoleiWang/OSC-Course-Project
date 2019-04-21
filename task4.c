#include <stdio.h>  
#include <pthread.h>  
#include <semaphore.h>
#include "coursework.h"

#define MAX_BUFFER_SIZE 10
#define  MAX_NUMBER_OF_JOBS 50

void removeHere(struct queue *my_arr, int index);

sem_t empty,full;               //declare semaphore value "empty"，"full"
//pthread_mutex_t mutex;          //定义一个全局互斥量，在不同函数中
int buffer_count=0;             //represent the products in buffer

struct queue q;
struct element e[MAX_NUMBER_OF_JOBS];

double AverR, AverT; 
int AverRes[MAX_NUMBER_OF_JOBS];
int AverTA[MAX_NUMBER_OF_JOBS];

int index1=0;
int p = 0;
int c = 0;
	
void *producer(void *arg ); 
void *consumer(void *arg );  

int main(int argc , char *argv[]){  
     

	init(&q, MAX_BUFFER_SIZE);
	printf("Max buffer: %d\n", MAX_BUFFER_SIZE);
	printf("Max number of jobs: %d\n", MAX_NUMBER_OF_JOBS);	
	
	pthread_t thrd_prod, thrd_cons; 

    //pthread_mutex_init(&mutex, NULL );    //初始化互斥量  
    sem_init (&empty, 0, MAX_BUFFER_SIZE);  //initializa "empty" semaphore
    sem_init (&full, 0, 0);                 //initializa "full" semaphore
    //vreate comsumer and producer threads 
    if(pthread_create( &thrd_prod, NULL, producer, NULL ) != 0 )  
        printf("thread create failed." ); 

    if(pthread_create( &thrd_cons, NULL, consumer, NULL ) != 0 )  
        printf("thread create failed." );  

    //waiting for thread finish  
    if(pthread_join( thrd_prod, NULL ) != 0 )  
        printf("wait thread failed.");  
    if(pthread_join( thrd_cons, NULL ) != 0 )  
        printf("wait thread failed.");  
    sem_destroy(&full);             //free semaphore
    sem_destroy(&empty);            //free semaphore
    //pthread_mutex_destroy(&mutex );        //关闭互斥量  


	printf("\nAverage response time: %.2f milliseconds\n", AverR/MAX_NUMBER_OF_JOBS);
	printf("Average turn around time: %.2f milliseconds\n", AverT/MAX_NUMBER_OF_JOBS);

	freeAll(&q);

	return 0;  
}  


void *producer(void *arg){  
    while(p<MAX_NUMBER_OF_JOBS){ /////1 
        sem_wait(&empty);  //empty-1 
        
	e[p] = generateProcess();

	for(int j=0; j<q.count; j++)
	{			
		if(e[p].pid_time < q.e[j].pid_time)////>
		{
			index1++;//printf("j is %d .", j);
		}
	}
	addHere(&q, &e[p], index1);
	//printAll(&q);
	//printf("q.count is %d, index1 is %d, e[p].pid_time is %d\n", q.count, index1, e[p].pid_time);
	p++;	
        buffer_count++;

	//printf("insex1 is %d, index is %d, q.count is %d\n", index1, index, q.count);
	printf("P: job on buffer %d, job produced %d, job consumed %d\n", buffer_count, p, c);
   	index1=0; //!!!

        sem_post(&full);        //full+1 

    }  
}



void *consumer(void *arg ){  
    while(c<MAX_NUMBER_OF_JOBS)
        {
        sem_wait(&full); //full-1

	struct element temp;
	temp = q.e[q.count-1];
	int t = q.count-1;
	removeHere(&q, q.count-1);   //remove

	struct timeval start, end;
	gettimeofday(&start, NULL);

	printf("Running index is %d.   ", t);
	runNonPreemptiveJobv2(&temp);   //run

	gettimeofday(&end, NULL);

	AverR += (int) getDifferenceInMilliSeconds(temp.created_time, start);
	AverT += (int) getDifferenceInMilliSeconds(temp.created_time, end);
	
	c++;
	buffer_count--;
	printf("C: job on buffer %d, job produced %d, job consumed %d\n", buffer_count, p, c); 
        
        sem_post(&empty);        //empty-1 

    }
}




void removeHere(struct queue *my_arr, int index)
{
	if(my_arr->e == NULL)
  	{
    		printf("removeHere: Couldn't find element!\n");
    		exit(1);
  	}
  	if(my_arr->count == 0)
  	{
    		printf("removeHere: element is empty!\n"); 
    		exit(1);
  	}
  	else
  	{
    		for(int i=index; i<my_arr->count-1; i++)       //each exist elements move 
    		{
      			my_arr->e[i] = my_arr->e[i+1];
    		}
		my_arr->e[my_arr->count-1].pid = 0;
    		my_arr->e[my_arr->count-1].pid_time = 0;
    		my_arr->e[my_arr->count-1].pid_priority = 0;
    		my_arr->e[my_arr->count-1].created_time.tv_sec = 0;
    		my_arr->e[my_arr->count-1].created_time.tv_usec = 0;
    		my_arr->count--;
  	}
}
