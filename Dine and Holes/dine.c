#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

//globals of the eats and philospehers
pthread_mutex_t lock[20000000];
/*
need thesee global varibales because what I am doing is to create two functions one that dines and one that
will see if it is ready to eat, the function for ready to eat will need the global variables for looping through the correct
ammount of times
*/

int eatTrack;
int philTrack;

void dinnerFunc(int numOfEating, int numOfPhil);
void *readyToEat(void * eaterId);

void dinnerFunc(int numOfEating, int numOfPhil) {

    //create array
	  eatTrack = numOfEating;
    philTrack = numOfPhil;

    pthread_t people[numOfPhil];


    for (int i = 0; i <= numOfPhil; i++) {
        //mutex Var
        pthread_mutex_init(&lock[i], NULL);
    }

    for (long i = 0; i < numOfPhil; i++) {
        //thread creater
        pthread_create(&people[i], NULL, readyToEat,(void*)i);
    }


    for (int j = 0; j < numOfPhil; j++) {
        //join and exit once everything is looped through
        pthread_join(people[j], NULL);
    }
}
void *readyToEat(void * eaterId) {

	  long ider;
    ider = (long)eaterId;

    int i = 0;




    printf("Philosopher %ld thinking\n", ider+1);

    //loop through the eattracker
    while (i < eatTrack) {

        //used the sleep function in order to wait for a current thread for a specified time.
        sleep(1);

        int temp = 0;

        if (ider != 0) {
            temp = ider-1;

        }
        else if(ider==0) {
          temp = philTrack-1;

        }


        int lockerId = pthread_mutex_lock(&lock[ider]);
        int lockerTemp = pthread_mutex_lock(&lock[temp]);


        if (lockerTemp == 0 && lockerId == 0) {
            //when locked its eating
            printf("Philosopher %ld eating\n", ider+1);
            i = i + 1;
            sleep(1);
        }

        printf("Philosopher %ld thinking\n", ider+1);


        pthread_mutex_unlock(&lock[ider]);
        pthread_mutex_unlock(&lock[temp]);
        //unlock and end of loop
    }

    //exit thread
    pthread_exit(NULL);


}
int main(int argc, char * argv[]) {

	//error check
	if (argc != 3) {

		printf("Invalid number of parameters\n");
		return 1;
	}

	//Begin simulation
	dinnerFunc(atoi(argv[2]), atoi(argv[1]));

  // printf("make file works for dine\n");

	return 0;
}
