#include "osc_queue.h"


int addFirst(struct queue *my_arr, struct element *new_e)
{
	if(my_arr->e == NULL)
	{
	        printf("Couldn't find element!\n");
		return 1;
	}
	if(my_arr->count < my_arr->max)//what is max mean?!
	{
	  for(int i=my_arr->count; i>0; i--)       //each exist elements move 
	  {
	    my_arr->e[i] = my_arr->e[i-1];
	  }
	        my_arr->count = my_arr->count + 1;
		my_arr->e[0] = *new_e;
		return 0;
	} 
	else
	{
		printf("The queue overflow!\n");
		return 1;
	}

}





int addHere(struct queue *my_arr, struct element *new_e, int index)
{
  	if(my_arr->e == NULL)
	{
		printf("Initialision fault in addHere!\n");
    		return 1;
  	}
  	if(my_arr->count >= my_arr->max)//what is max mean?!
  	{
    		printf("The queue overflow!\n");
    		return 1;
  	}
  	else
  	{
    		for(int i=my_arr->count; i>index; i--)       //each exist elements move 
    		{
      			my_arr->e[i] = my_arr->e[i-1];
    		}
    		my_arr->e[index] = *new_e;
    		my_arr->count++;
    		return 0;
  	}
}



int addLast(struct queue *my_arr, struct element *new_e)
{
  	if(my_arr->e == NULL)
  	{
    		printf("Initialision fault in addLast!\n");
    		return 1;
  	}
  	if(my_arr->count >= my_arr->max)//what is max mean?!
  	{
    		printf("The queue overflow!\n");
    		return 1;
  	}
  	else
  	{
    		my_arr->e[my_arr->count] = *new_e;
    		my_arr->count++;
    		return 0;
  	}
}



void freeAll(struct queue *my_arr)
{
	my_arr->e = NULL;
	free(my_arr->e);
        
	//free(my_arr);
}



int getCount(struct queue *my_arr)
{
	return my_arr->count;
}



int init(struct queue *my_arr, int arr_size)
{
        printf("Initialising ...\n");
	//my_arr = (struct queue*)malloc(sizeof(struct queue));
	my_arr->max = arr_size;
	my_arr->count = 0;
	my_arr->e = (struct element*)malloc(arr_size * sizeof(struct element));   //(e*)
	for(int i=0; i<arr_size; i++)
	{
	   my_arr->e[i].pid = 0;
	   my_arr->e[i].pid_time = 0;
	   my_arr->e[i].pid_priority = 0;
	   my_arr->e[i].created_time.tv_sec = 0;
	   my_arr->e[i].created_time.tv_usec = 0;
	}

	if(my_arr->e)
	{
		printf("Init: successfully malloc element with size of %d ..\n", arr_size);
	}
	else
	{
		printf("allocate memory fail!\n");
		return 1;
	}
}



void printAll(struct queue *my_arr)
{
  	if(my_arr->e == NULL)
  	{
    		printf("Initialision fault in printAll!\n");
    		exit(1);
  	}
 
  	printf("There are %d elements in total\n", my_arr->count);

  	for(int index=0; index<my_arr->max; index++)/////////////////////
  	{
    		printf("#[%d]: %d running-time %d created-time %d sec %d usec %d priority\n", index, my_arr->e[index].pid, my_arr->e[index].pid_time, my_arr->e[index].created_time.tv_sec, my_arr->e[index].created_time.tv_usec, my_arr->e[index].pid_priority);
  	}
}


void removeLast(struct queue *my_arr)
{
  	if(my_arr->e == NULL)
  	{
    		printf("removeLast: Couldn't find element!\n");
    		exit(1);
  	}
  	if(my_arr->count == 0)
  	{
    		printf("removeLast: element is empty!\n"); 
    		exit(1);
  	}
  	else
  	{
    		my_arr->e[my_arr->count-1].pid = 0;
    		my_arr->e[my_arr->count-1].pid_time = 0;
    		my_arr->e[my_arr->count-1].pid_priority = 0;
    		my_arr->e[my_arr->count-1].created_time.tv_sec = 0;
    		my_arr->e[my_arr->count-1].created_time.tv_usec = 0;
    		my_arr->count--;
  	}
}

