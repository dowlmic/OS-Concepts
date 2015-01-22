#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

pid_t child;

void GracefuleClose(int sig){
    printf ("GRACEFUL SHUTDOWN\n");
    kill(child, SIGKILL);
    // this is where shutdown code would be inserted
    exit(0); 
}

void UserSignal(int sig) {
    if(sig == 10)
	printf("Received SIGUSR1 \n");
    else
	printf("Received SIGUSR2 \n");
    fflush(stdout);
}

int main()
{ 

    // use these variables
    pid_t pid, parent;
    parent = getpid();
    if ((pid = fork()) < 0) {
        perror("fork failure");
        exit(1);
    }
    else if (pid == 0) {
	printf("Child Created\n");
	fflush(stdout);
        // Generate SIGNAL HERE
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

	        //write (STDOUT_FILENO, (const void *) str, (size_t) strlen (str) + 1);
	}
    }
    else {
	printf("Parent Waiting\n");
	fflush(stdout);
	child = pid;
	signal(SIGINT, GracefuleClose);
        signal(SIGUSR1, UserSignal);
	signal(SIGUSR2, UserSignal);
	while(1);
	//read (STDIN_FILENO, (void *) str, (size_t)  sizeof (str));
	// REACT TO SIGNAL HERE
    }
    return 0;
} 
