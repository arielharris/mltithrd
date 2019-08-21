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
int totalPhilo;

void
sectionInitGlobals(int numPhilosophers)
{
	const char* TRACE = "Lock trace";
	int i;
	
	mdat_mutex_init(TRACE, &lock, NULL);
	chopsticks = (int*) malloc(numPhilosophers* sizeof(int));
	for(i = 0; i < numPhilosophers; i++)
		chopsticks[i] = 0;

	totalPhilo = numPhilosophers;
}

void
sectionRunPhilosopher(int philosopherID, int numRounds)
{
	int i = 0;
	int bothSticks = 0;
	int eaten; 
	
	while (i < numRounds){
		
		//the highest num philosopher gets highest num chopstick
		if (philosopherID == (totalPhilo-1)){
			mdat_mutex_lock(&lock);
			chopsticks[philosopherID] = 1;
			bothSticks++;
			mdat_mutex_unlock(&lock);
		} else { //otherwise pick up lowest num stick of two options if avilable
			if (chopsticks[philosopherID] == 0){
				mdat_mutex_lock(&lock);
				chopsticks[philosopherID] = 1;
				bothSticks++;
				mdat_mutex_unlock(&lock);
			}
		}
		
		//then try and grab highest of two sticks or 0 for highest philosopher
		if (philosopherID == (totalPhilo-1)){
			mdat_mutex_lock(&lock);
			chopsticks[0] = 1;
			bothSticks++;
			mdat_mutex_unlock(&lock);
		} else {
			if (chopsticks[philosopherID + 1] == 0){
				mdat_mutex_lock(&lock);
				chopsticks[philosopherID + 1] = 1;
				bothSticks++;
				mdat_mutex_unlock(&lock);
			}
		}
		
		if (bothSticks == 2){
			eat();
			eaten = 1;
			mdat_mutex_lock(&lock);
			chopsticks[philosopherID] = 0;
			chopsticks[philosopherID + 1] = 0;
			bothSticks = 0;
			mdat_mutex_unlock(&lock);
		}
		
		if (eaten == 1){
			think();
			eaten = 0;
			i++;
		}
			
	}
}
