#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include "part1.h"
#include "main.h"


#define MAX_NUM_FLOORS 20
int maxPeople;
int numOfFloors;
int peopleInElevator;
int totalPeopleWaiting;
int peopleOnFloor[MAX_NUM_FLOORS];
int peopleWithDest[MAX_NUM_FLOORS];//2d



sem_t* sem_in;

sem_t* sem_out;
sem_t* lock;
sem_t* forLeaving;
sem_t* forEntering;


void initializeP1(int numFloors, int maxNumPeople) {

	lock=malloc(sizeof(sem_t));
	forLeaving=malloc(sizeof(sem_t));
	forEntering=malloc(sizeof(sem_t));
	

	sem_in=malloc(sizeof(sem_t)*numFloors);
	sem_out=malloc(sizeof(sem_t)*numFloors);
	int i=0;
	for(i=0;i<numFloors;i++) {
		sem_init(&sem_in[i],0,0);
		sem_init(&sem_out[i],0,0);
		peopleOnFloor[i]=0;
		peopleWithDest[i]=0;
	}
	sem_init(lock,0,1);
	sem_init(forLeaving,1,0);
	sem_init(forEntering,1,0);
	numOfFloors=numFloors;
	maxPeople=maxNumPeople;
	peopleInElevator=0;
	totalPeopleWaiting=0;

	return;
}




void* goingFromToP1(void *arg) {

	struct argument *user = (struct argument *) arg;


	int x[3];
	x[0]=user->from;
	x[1]=user->to;
	x[2]=user->id;

	
	sem_wait(lock);
		peopleOnFloor[x[0]]++;
		totalPeopleWaiting++;
	sem_post(lock);

	sem_wait(&sem_in[x[0]]);	

	if (peopleInElevator<maxPeople) {
		
		sem_wait(lock);
			
			peopleOnFloor[x[0]]--;
			totalPeopleWaiting--;
			peopleInElevator++;
			//printf("IN: %d %d %d\n",x[0],x[1],x[2]);
			if(peopleOnFloor[x[0]]>0 && peopleInElevator<maxPeople) {
				sem_post(&sem_in[x[0]]);
			} else {
				sem_post(forEntering);
			}
		
			peopleWithDest[x[1]]++;
		sem_post(lock);
		
		sem_wait(&sem_out[x[1]]);//2d waiting in train x for station y
	
		sem_wait(lock);
			peopleInElevator--;
			peopleWithDest[x[1]]--;
			printf("%d %d %d\n",x[2], x[0], x[1]);
			if(peopleWithDest[x[1]]>0) {
				sem_post(&sem_out[x[1]]);
			} else {
				sem_post(forLeaving);
			}
		sem_post(lock);
		
	} else {	//there is no space in elevator
		do {		
			sem_wait(&sem_in[x[0]]);
		} while(peopleInElevator>=maxPeople);
		
		sem_wait(lock);
			peopleOnFloor[x[0]]--;
			totalPeopleWaiting--;
			peopleInElevator++;
			//printf("IN*: %d %d %d\n",x[0],x[1],x[2]);
			if(peopleOnFloor[x[0]]>0 && peopleInElevator<maxPeople) {
				sem_post(&sem_in[x[0]]);
			} else {
				sem_post(forEntering);
			}

			peopleWithDest[x[1]]++;
		sem_post(lock);

		sem_wait(&sem_out[x[1]]);

		sem_wait(lock);
			peopleInElevator--;
			peopleWithDest[x[1]]--;
			printf("%d %d %d\n",x[2], x[0], x[1]);
			if(peopleWithDest[x[1]]>0) {
				sem_post(&sem_out[x[1]]);
			} else {
				sem_post(forLeaving);
			}
		sem_post(lock);

	}

	return NULL;
}

/*If you see the main file, you will get to 
know that this function is called after setting every
passenger.

So use this function for starting your elevator. In 
this way, you will be sure that all passengers are already
waiting for the elevator.
*/
void startP1(){
	sleep(4);
	//printf("ELEVATOR STARTED\n");
	while(totalPeopleWaiting>0 || peopleInElevator>0) {	//keep running elevator while people are waiting outside or inside elevator
		
		
		int i=0;
		for(i=0;i<numOfFloors;i++) {
				
			if(peopleWithDest[i]>0) {
				sem_post(&sem_out[i]);
				sem_wait(forLeaving);
			}
			if(peopleOnFloor[i]>0) {
				sem_post(&sem_in[i]);
				sem_wait(forEntering);
			}	
		}
		
		for(i=numOfFloors-1;i>=0;i--) {
			
			if(peopleWithDest[i]>0) {
				sem_post(&sem_out[i]);
				sem_wait(forLeaving);
			}
			if(peopleOnFloor[i]>0) {
				sem_post(&sem_in[i]);
				sem_wait(forEntering);
			}	

		}
	}
}


/*int main() {
	initializeP1(4,1);

	pthread_t tid, tid2, tid3;

	
	struct argument *arg = (struct argument*)malloc(sizeof(struct argument));
        arg->from = 0;
        arg->to = 1;
        arg->id = 2;
	//goingFromToP1((void*) arg);
        pthread_create(&tid, NULL, goingFromToP1, (void*) arg);
	
	struct argument *arg2 = (struct argument*)malloc(sizeof(struct argument));
	arg2->from=1;
	arg2->to=0;
	arg2->id=3;
	pthread_create(&tid2, NULL, goingFromToP1, (void*) arg2);
	//pthread_create(&tid,&attr,goingFromToP1,args);

	struct argument *arg3 = (struct argument*)malloc(sizeof(struct argument));
	arg3->from=0;
	arg3->to=1;
	arg3->id=4;
	pthread_create(&tid3, NULL, goingFromToP1, (void*) arg3);

	startP1();
	pthread_join(tid3,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid,NULL);
	
	return 0;
}*/
