#include <stdio.h>
#include "coursework.h"
#include <unistd.h>

#define MAX_PROCESSES 5


int main(void)
{
	
  struct queue myarr;
	// = (struct queue*)malloc(sizeof(struct queue));//////
  init(&myarr, MAX_PROCESSES);


  printAll(&myarr);


  //////////////FIFO

  printf("Performing FIFO\n");

  
  for(int index=0; index<MAX_PROCESSES; index++)
  {
    struct element ele = generateProcess();
    addFirst(&myarr, &ele);
    printf("Add: pid %d into the queue with time %d\n", myarr.e[0].pid, myarr.e[0].pid_time);////

  }
  
  printAll(&myarr);


  for(int i=0; i<MAX_PROCESSES; i++)
  {
    printf("Remove: %d from the queue index %d\n", myarr.e[MAX_PROCESSES-i-1].pid, MAX_PROCESSES-i-1);

    removeLast(&myarr);
    printAll(&myarr);

  }



  //////////LIFO

  printf("Performing FILO\n");

  
  for(int i=0; i<MAX_PROCESSES; i++)
  {
    struct element ele = generateProcess();
    addLast(&myarr, &ele);
    printf("Add: pid %d into the queue with time %d\n", myarr.e[i].pid, myarr.e[i].pid_time);////
 
  }
  
  printAll(&myarr);


  for(int i=0; i<MAX_PROCESSES; i++)
  {
    printf("Remove: %d from the queue index %d\n", myarr.e[MAX_PROCESSES-i-1].pid, MAX_PROCESSES-i-1);

   
    removeLast(&myarr);
    printAll(&myarr);

  }
 
  freeAll(&myarr);

  return 0;

}




