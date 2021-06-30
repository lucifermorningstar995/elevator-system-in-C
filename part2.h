#ifndef PART_2
#define PART_2

/**
 * Do any initial setup work in this function.
 * numStations: Total number of stations. Will be >= 5
 * maxNumPeople: The maximum number of people in a train
 */
 
 struct train_flow{
 	int number;
 	//int station;
 	//int total_cap;
 	//int current_num_people;
 	};
void initializeP2(int numTrains, int numStations);
/**
 * Print data in the format described in part 5
 */
void * goingFromToP2(void * user_data);


void * startP2();
#endif
