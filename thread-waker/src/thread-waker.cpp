#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
sem_t * sem;

int main(void) {
	int numWakeup;

	// Call the function to open a named semaphore
	// The named semaphore has already been created so only 2 params are needed: nameOfYourSemaphore and 0 (zero)
	sem = sem_open("tfactory", 0);
	// Display the PID of the thread-waker process
	printf("thread-waker PID: %d\n",getpid());

	do{
		numWakeup = 0;
		// Request the number of threads to wake-up. If invalid data in entered, exit the loop.
		printf("How many threads would you like to wake up (enter 0 to exit): ");
		scanf("%d", &numWakeup);

		for(int i=0;i<numWakeup;i++){
			//call the function to unlock the semaphore
			sem_post(sem);
			printf("posted");
		}

	}while(numWakeup!=0);

	// Close and destroy the semaphore
	sem_close(sem);
	sem_destroy(sem);

	exit(-1);
}

