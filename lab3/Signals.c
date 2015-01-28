/* Signals.c
 *
 * Authors: Lucas Ordaz and Michelle Dowling
 *
 * This program is designed to show how to send and receive signals between two
 * processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

// Global variable to store the PID of the child thread
pid_t child;

// Method to ensure a gracefull shutdown
void GracefuleClose(int sig){
    printf (" received. That't it, I'm shutting down\n");
    kill(child, SIGKILL);
    exit(0); 
}

// Method to handle incoming user signals
void UserSignal(int sig) {
    if(sig == 10)
	printf("received a SIGUSR1 signal\nwaiting...\t");
    else
	printf("received a SIGUSR2 signal\nwaiting...\t");
    fflush(stdout);
}

int main()
{ 

    // Variables to store the current PID and the parent's PID
    pid_t pid, parent;
    parent = getpid();

    // Check to make sure that the fork executed properly
    if ((pid = fork()) < 0) {
        perror("fork failure");
        exit(1);
    }

    // Code for the child process
    else if (pid == 0) {

        // Generate random signal after a random interval
	srand(time(NULL));
	int r;
	int sleepTime;
        while(1) {
		r = rand() % 2;
		sleepTime = (rand() % 4)+1;
		if(r == 0)
			kill(parent, SIGUSR1);
		else
			kill(parent, SIGUSR2);
		printf("Sleeping %d", sleepTime);
		sleep(sleepTime);
	}
    }

    // Parent code
    else {
        // Print info
	printf("spawned child PID# %d\n", pid);
	printf("waiting...\t");
	fflush(stdout);

        // Set child PID and set up signal handlers
	child = pid;
	signal(SIGINT, GracefuleClose);
        signal(SIGUSR1, UserSignal);
	signal(SIGUSR2, UserSignal);
	while(1);
    }
    return 0;
} 
