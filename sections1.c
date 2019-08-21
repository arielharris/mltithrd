//  sections1.c: mutual exclusion model sections

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "sections.h"
#include "mdat.h"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock;
int* chopsticks;

void
sectionInitGlobals(int numPhilosophers)
{
	const char* TRACE = "Lock trace";
	int i;
	
	mdat_mutex_init(TRACE, &lock, NULL);
	chopsticks = (int*) malloc(numPhilosophers* sizeof(int));
	for(i = 0; i < numPhilosophers; i++)
		chopsticks[i] = 0;

}

void
sectionRunPhilosopher(int philosopherID, int numRounds)
{
	int i;
	int bothSticks = 0;
	
	for (i = 0; i < numRounds; i++){
		//get left
		if (chopsticks[philosopherID] == 0){
			mdat_mutex_lock(&lock);
			chopsticks[philosopherID] = 1;
			bothSticks++;
		}
		
		//get right
		if (chopsticks[philosopherID + 1] == 0){
			mdat_mutex_lock(&lock);
			chopsticks[philosopherID + 1] = 1;
			bothSticks++;
		}
		
		if (bothSticks == 2){
			eat();
			chopsticks[philosopherID] = 0;
			chopsticks[philosopherID + 1] = 0;
			mdat_mutex_unlock(&lock);
			think();
		}
			
	}
}
