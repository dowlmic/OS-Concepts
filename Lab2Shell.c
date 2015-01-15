#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{ 

    // use these variables

    pid_t pid, child;
    int status;
    

    while(1)
    {
	char command[80];
	printf("Command> ");
	scanf("%s", command);

	if(strcmp("quit", command) == 0) // Exit on Quit
		exit(0);

	char * prog = strtok(command, " "); // Gets the program
	//printf("%s", command);
	printf("%s", prog);

        if ((pid = fork()) < 0) { // Creates the Child
            perror("fork failure");
            exit(1);
        }
        else if (pid == 0) { // If Child
//            execvp();
   	    exit(0);

        }
        else {
//	    wait(&status);
        }
    }
    return 0;
} 
