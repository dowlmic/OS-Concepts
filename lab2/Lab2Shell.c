/* Lab 2 Program: Simple Shell
 * Authors: Lucas Ordaz and Michelle Dowling
 * 
 * This program is designed to simulate a simple shell for the user as well as
 * reporting resource utilization statistics for each command run
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>

int main()
{

    // use these variables
    pid_t pid, child;
    int status;
    char input[100]; // User input
    struct rusage usage; // struct to pass to wait3 to get the system resource use

    // Prompt the user for a command
    printf("Welcome to Simple Shell! You can enter in basic commands, or quit using \"quit\"\n\n");

    while(1)
    {
        memset(input, '\0', sizeof input); // Reset Input String

        fgets(input, sizeof input, stdin);
        if(strcmp("quit\n", input) == 0) // Exit on Quit
            exit(0);

        if ((pid = fork()) < 0) { // Creates the Child
            perror("fork failure");
            exit(1);
        }
        else if (pid == 0) { // If Child;
            char* param_array[100] = {NULL}; // Initialize array to null
            char * token = strtok(input, " \n"); // get token
            int index = 0;
            while (token != NULL) // Convert input into array of strings
            {
                param_array[index] = malloc(strlen(token));
                param_array[index] = token;
                token = strtok(NULL, " \n");
                index++;
            }
            param_array[index] = NULL; // End the Array
            execvp(param_array[0], param_array); // Execute the program
            memset(param_array, '\0', sizeof param_array); // Reset the memory for the array
            exit(0);
        }
        else {
            // Using resource.h and getrusage to gather the system resources
            // used. wait3 can fill that in automatically

            // Wait for the child process to end and get the status and resource usage
            child = wait3(&status,0, &usage);

            // Print the resource usage
            printf("Involuntary Context Switches: %ld\n", usage.ru_nivcsw);
            printf("System CPU Time Used: %ldus\n", usage.ru_stime.tv_usec);
            printf("User CPU Time Used: %ldus \n\n", usage.ru_utime.tv_usec);
        }
    }
    return 0;
}
