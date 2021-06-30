#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "part2.h"
#include "main.h"
#include <stdlib.h>
#include <unistd.h>

/*
	- DO NOT USE SLEEP FUNCTION
	- Define every helper function in .h file
	- Use Semaphores for synchronization purposes
 */


/**
* Declare semaphores here so that they are available to all functions.
*/
// sem_t* example_semaphore;
const int INTER_ARRIVAL_TIME = 5;
const int NUM_TRAINS = 5;
// int* people_in_Station;
// int* people_with_dest







sem_t* sem_people_scan;




sem_t* sem_totalpeopleinTrain;

sem_t* sem_train_id;
sem_t* sem_traininstation;
sem_t* sem_peoplewaitinginstation;
sem_t* sem_people_entering_train;
sem_t* sem_peoplewaitingfordest;
sem_t* sem_out;
sem_t* sem_in;
sem_t* sem_totalpeoplewaiting;


sem_t* lock;
int* peoplewaitinginstation;
int* peoplewaitingfordest;
int start;
int finish;
int* train_id_station;
int max_people;
int Numofstations;
int Totalpeoplewaiting;
int TotalpeopleinTrain;
/**
 * Do any initial setup work in this function. You might want to 
 * initialize your semaphores here. Remember this is C and uses Malloc for memory allocation.
 *
 * numStations: Total number of stations. Will be >= 5. Assume that initially
 * the first train is at station 1, the second at 2 and so on.
 * maxNumPeople: The maximum number of people in a train
 */
void initializeP2(int numStations, int maxNumPeople) {
	// example_semaphore = (sem_t*) malloc(sizeof(sem_t));
	
	start=0;
	finish=1;

	max_people=maxNumPeople;
	Numofstations=numStations;

	Totalpeoplewaiting=0;
	TotalpeopleinTrain=0;
	int i;
	int j;
	train_id_station=(int *)malloc(sizeof(int)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		train_id_station[i]=0;//0/////////////////////////////////////////////////////////////////////////////
	}

	peoplewaitinginstation=(int *)malloc(sizeof(int)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		peoplewaitinginstation[i]=0;
	}

	sem_peoplewaitingfordest=malloc(sizeof(int)*Numofstations*NUM_TRAINS);
	for (i=0; i<NUM_TRAINS; i++)
	{
		for(j=0; j<Numofstations;j++)
		{
			sem_init(&sem_peoplewaitingfordest[i*Numofstations + j],0,0);
		}
	}

	sem_peoplewaitinginstation=malloc(sizeof(sem_t)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		sem_init(&sem_peoplewaitinginstation[i],0,0);
	}

	sem_in=malloc(sizeof(sem_t)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		sem_init(&sem_in[i],0,0);
	}

	sem_out=malloc(sizeof(sem_t)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		sem_init(&sem_out[i],0,0);
	}

	sem_totalpeoplewaiting=malloc(sizeof(sem_t));
	sem_init(sem_totalpeoplewaiting,0,1);

	sem_traininstation=malloc(sizeof(sem_t)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		sem_init(&sem_traininstation[i],0,1);
	}
	sem_people_entering_train=malloc(sizeof(sem_t)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		sem_init(&sem_people_entering_train[i],0,1);
	}

	sem_totalpeopleinTrain=malloc(sizeof(sem_t));
	sem_init(sem_totalpeopleinTrain,0,1);

	sem_train_id=malloc(sizeof(sem_t));
	sem_init(sem_train_id,0,1);

	sem_people_scan=malloc(sizeof(sem_t)*Numofstations);
	for(i=0;i<Numofstations;i++)
	{
		sem_init(&sem_people_scan[i],0,1);
	}
	lock=malloc(sizeof(sem_t));
	sem_init(lock,0,1);

	peoplewaitingfordest=(int *)malloc(sizeof(int)*Numofstations*NUM_TRAINS);
	for (i=0; i<NUM_TRAINS; i++)
	{
		for(j=0; j<Numofstations;j++)
		{
			peoplewaitingfordest[i*Numofstations + j]=0;
		}
	}

}
	

/**
	This function is called by each user.

 * Print in the following format:
 * If a user borads on train 0, from station 0 to station 1, and another boards
 * train 2 from station 2 to station 4, then the output will be
 * 0 0 1
 * 2 2 4
 */
void * goingFromToP2(void * user_data) {
	struct argument *user=(struct argument *) user_data;
	int train_id;
	int x[3];
	x[0]=user->from;
	x[1]=user->to;
	x[2]=user->id;
	
	
	//printf("%d %d %d\n",x[2],x[0],x[1]);
	sem_wait(&sem_people_scan[x[0]]);
		peoplewaitinginstation[x[0]]++;
	sem_post(&sem_people_scan[x[0]]);

	sem_wait(sem_totalpeoplewaiting);
		Totalpeoplewaiting++;
	sem_post(sem_totalpeoplewaiting);

	sem_wait(&sem_peoplewaitinginstation[x[0]]);


	sem_wait(sem_totalpeoplewaiting);
		Totalpeoplewaiting--;
	sem_post(sem_totalpeoplewaiting);


	sem_wait(&sem_people_scan[x[0]]);
		peoplewaitinginstation[x[0]]--;
	sem_post(&sem_people_scan[x[0]]);

	sem_wait(sem_totalpeopleinTrain);
		TotalpeopleinTrain++;
	sem_post(sem_totalpeopleinTrain);


	sem_wait(&sem_people_entering_train[x[0]]);
		train_id=train_id_station[x[0]];
		peoplewaitingfordest[train_id*Numofstations + x[1]]++;
	sem_post(&sem_people_entering_train[x[0]]);

	
	sem_post(&sem_in[x[0]]);





	sem_wait(&sem_peoplewaitingfordest[train_id*Numofstations+x[1]]);

		
	sem_wait(sem_totalpeopleinTrain);
		TotalpeopleinTrain--;
		printf("%d %d %d\n", x[2],x[0],x[1]);
	sem_post(sem_totalpeopleinTrain);


	sem_wait(&sem_people_entering_train[x[1]]);
		peoplewaitingfordest[train_id*Numofstations + x[1]]--;
	sem_post(&sem_people_entering_train[x[1]]);


	
	
	start++;
	finish++;


	sem_post(&sem_out[x[1]]);
	



	return NULL;

}

void* train_fun(void* args)
{
	// struct argument* train_thread=(struct argument*)args;
	// //printf("%d %d %d\n",train_thread -> id,train_thread->from,train_thread->to);
	// int i;
	// while(Totalpeoplewaiting>0 || people_in_train>0)

	// {
	// 	//i=train_thread->to;

	// 	if(people_with_dest[i]>0)
	// 	{

	// 	}
	// 	if (people_in_Station[i]>0)
	// 	{
	// 		sem_post(&sem_in[i]);
	// 		sem_wait(forEntering);
	// 	}
	// 	i=(i+1)%Numofstations;
	// }

	// return NULL;
	//sem_wait(lock);
	struct train_flow *new_train=(struct train_flow *)args;
	int number=new_train->number;
	int curr_station=new_train->number;

	while(Totalpeoplewaiting >0 || TotalpeopleinTrain>0)
	{
		int i;
		
		
		
		sem_wait(&sem_traininstation[curr_station]);
		if(peoplewaitinginstation[curr_station]>0 || peoplewaitingfordest[number*Numofstations+curr_station]>0)
		{
			sem_wait(sem_train_id);
			train_id_station[curr_station]=number;
			sem_post(sem_train_id);
			
			for(i=0;i<peoplewaitingfordest[number*Numofstations+curr_station];i++)
			{
				sem_post(&sem_peoplewaitingfordest[number*Numofstations+curr_station]);
				sem_wait(&sem_out[curr_station]);

			}

			for(i=0;i<peoplewaitinginstation[curr_station];i++)
			{
				sem_post(&sem_peoplewaitinginstation[curr_station]);
				sem_wait(&sem_in[curr_station]);
			}

		}
		curr_station=(curr_station+1)%Numofstations;

		sem_post(&sem_traininstation[curr_station]);


		
		

	}
	sem_post(lock);
	return NULL;
	
	
}


 //Use this function to start threads for your trains 
void * startP2(){
	// sleep(INTER_ARRIVAL_TIME);
	// pthread_t thread[NUM_TRAINS];
	// int j;
	// for (j=0;j<NUM_TRAINS;j++)
	// {
	// 	pthread_create(&thread[j],NULL,train_func,(void*)&train[j]);
	// }
	// return NULL;
	//sem_wait(lock);
	sleep(5);
	pthread_t trainthread[NUM_TRAINS];
	
	int i;
	for (i=0;i<NUM_TRAINS;i++)
	{
		pthread_t tid;
		struct train_flow *thread_train=(struct train_flow*)malloc(sizeof(struct train_flow));
		thread_train->number=i;
				
		sem_wait(lock);
		pthread_create(&tid, NULL,train_fun,(void*)thread_train);
		
		trainthread[i]=tid;
	}
	

}