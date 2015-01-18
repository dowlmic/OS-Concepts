#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main()
{

    // use these variables
    pid_t pid, child;
    int status;
    char input[30];

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
        else if (pid == 0) { // If Child
            char* param_array[50] = {NULL}; // Initialize array to null
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
            child = wait(&status);
        }
    }
    return 0;
}
