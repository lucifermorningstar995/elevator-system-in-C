#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include "part3.h"
#include <unistd.h>

#include "main.h"





const int MAX_NUM_DIRECTIONS = 4;



sem_t *left,*right, *redorgreen, *mutex;
int* rightlane,*leftlane;
void initializeP3() {
	mutex=malloc(sizeof(sem_t));
	rightlane=(int*) malloc(sizeof(int)*MAX_NUM_DIRECTIONS);
	leftlane=(int*)malloc(sizeof(int)*MAX_NUM_DIRECTIONS);
	right=(sem_t*)malloc(sizeof(sem_t)*MAX_NUM_DIRECTIONS);
	left=(sem_t*)malloc(sizeof(sem_t)*MAX_NUM_DIRECTIONS);
	redorgreen =(sem_t*)malloc(sizeof(sem_t)*MAX_NUM_DIRECTIONS);
	sem_init(mutex,0,0);
	int i;	
	for (i=0;i<MAX_NUM_DIRECTIONS;i++)
	{
		sem_init(&redorgreen[i],0,0);
		sem_init(&right[i],0,0);
		sem_init(&left[i],0,0);
	}
   for(i=0;i<MAX_NUM_DIRECTIONS;i++)
   {
		leftlane[i]=0;
		rightlane[i]=0;
   }
	
}


void * goingFromToP3(void *argu){
	
    struct argumentP3* car=(struct argumentP3*) argu;
    enum DIRECTION f=car->from;
    enum DIRECTION t=car->to;
    enum LANE l=car->lane;

    
    	 if (l)
        {
    			rightlane[f]+=1;
    		 sem_wait(&right[f]);
    			rightlane[f]-=1;
    	 }
        else
        {
    			leftlane[f]+=1;
    		sem_wait(&left[f]);
    			leftlane[f]-=1;
    	 }

    	sem_post(&redorgreen[f]);
      
       char north[]="NORTH";
       char south[]="SOUTH";
       char east[]="EAST";
       char west[]="WEST";
       switch (f)
       {  
          case 0:
             printf("\r%s", north);
             break;
          case 1:
             printf("\r%s", south);
             break;
          case 2:
             printf("\r%s", east);
             break;
          case 3:
             printf("\r%s", west);
             break;
          default:
             break;
       }
       switch (t)
       {  
          case 0:
             printf("\r%s", north);
             break;
          case 1:
             printf("\r%s", south);
             break;
          case 2:
             printf("\r%s", east);
             break;
          case 3:
             printf("\r%s", west);
             break;
          default:
             break;
       }
       switch (l)
       {  
          case 0:
             printf("\r%s", "LEFT");
             break;
          case 1:
             printf("\r%s", "RIGHT");
             break;
          default:
             break;
       }
       sem_post(mutex);
    
}


void startP3(){
	sleep(4);

 int i;  
 for(i=0;i<MAX_NUM_DIRECTIONS;i++){
      int direction=0;
      if(i==0)
      {
         direction=2;
      }
      else if(i==1)
      {
         direction=1;
      }
      else if(i==2)
      {
         direction=0;
      }
      else if(i==3)
      {
         direction=3;
      }

      while(leftlane[direction]>0)
      {
         sem_post(&left[direction]);
         sem_wait(&redorgreen[direction]);
      }
      sem_wait(mutex);
      while(rightlane[direction]>0)
      {
         sem_post(&right[direction]);
         sem_wait(&redorgreen[direction]);
      }
   }
}
