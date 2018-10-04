#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>


sig_atomic_t handlerCalled = 0;
sem_t * sem;

void handler(int sig);
void* childThread(void* arg);

int main(void) {

	struct sigaction sa;
	int nThreads = -1;
	char buffer;

	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	// Collect a number for amount of threads created. Flush out invalid input.
	while (nThreads < 1){
		printf("Enter the number of threads to create: ");
		fflush(stdout);
		if (!scanf("%d", &nThreads)){
			while((buffer = getchar()) != '\n' && buffer != EOF);
		}
	}

	// Call the function to create the semaphore
	sem = sem_open("tfactory", O_CREAT, S_IWOTH,0);

	pthread_attr_t attr;
	for (int i = 0; i < nThreads; i++) {
		pthread_attr_init(&attr);
		pthread_create(NULL, &attr, &childThread, NULL);
		pthread_attr_destroy(&attr);
	}

	// Wait for the signal to be recieved
	while (!handlerCalled);

	// Close and destroy the semaphore
	sem_close(sem);
	sem_destroy(sem);

	exit(-1);

}

void handler(int sig) {
	if (sig == SIGUSR1) {
		handlerCalled = 1;
	}
}

void* childThread(void* arg) {

	printf("Child thread %d created\n",pthread_self());

	while(true){
		sem_wait(sem);
		printf("Child thread %d unblocked\n", pthread_self());
		// Pretend to be doing work
		sleep(5);
	}

	return NULL;
}
