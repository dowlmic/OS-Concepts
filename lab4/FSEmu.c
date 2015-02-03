/* FSEmu.c
 *
 * Authors: Lucas Ordaz and Michelle Dowling
 *
 * This program is designed to emulate a file system and user multiple threads
 * to retreive the files. Files that are 'cached' are retrieved faster than
  * 'non-cached' files.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

void* find_file(void* arg);
void graceful_close();
void* increment_access_time(char* arg, int time);

// Globals
int numRequests = 0; // Number of files requested
int requestFulfilled = 0; // Number of file requested fulfilled
int accessTimeSum = 0; // Sum of the file access time
pthread_mutex_t funcLock; // Locks the thread for concurrency

int main(){
    pthread_t thread;
    int status;
    char input[50];

    // Set up randomness
    srand(time(NULL));

    // Setup Handler for SIGINT
    signal(SIGINT, graceful_close);

    // Initializes mutex for use
    if(pthread_mutex_init(&funcLock, NULL) != 0){
        printf("Unable to initiate Mutex");
        exit(1);
    }

    // Continuously receive file requests
    while(1){
        fgets(input, 50, stdin);
        if ((status = pthread_create(&thread, NULL, find_file, input)) != 0) {
            fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
            exit(1);
        }
        if ((status = pthread_detach(thread)) != 0) {
            fprintf(stderr, "detach error %d: %s\n", status, strerror(status));
        }
        numRequests++;
    }

    pthread_mutex_destroy(&funcLock); // Clean up mutex if program exits normally
    exit(0);
}

// Finds the file requested
void* find_file(void* arg){
    // Copy filename from argument
    // Without it there is a race condition where the next request can overwrite the argument
    char file[50];
    strcpy(file, (char*)arg);
    // Simulate a chached file vs a non-cached file
    int chachedFile = rand() % 5;
    int sleepTime = 1;
    if (chachedFile <= 3) {
	sleep(sleepTime);
    }
    else {
            sleepTime = (rand() % 3) + 7;
            sleep(sleepTime);
    }

    // Simulate file retrieval
    increment_access_time(file, sleepTime);
    return NULL;
}

// SIGINT Hanlder, prints the number of files requested and then exits the program
void graceful_close(){
    float avgAccessTime = 0;
    // Print Stats
    printf (" received. %d files were requested. \n", numRequests);
    printf("Number of requests fullfiled %d\n", requestFulfilled);
    printf("Sum of access time %d\n", accessTimeSum);
    avgAccessTime = ((float)accessTimeSum/requestFulfilled);
    printf("Average Access Time %.2f\n", avgAccessTime);
    pthread_mutex_destroy(&funcLock); // Cleanup mutex
    exit(0);
}

// Increments the summation of file retrieval time
// Increments the summation of files fully retreived
void* increment_access_time(char* arg, int time){
    pthread_mutex_lock(&funcLock); // Locks the function
    printf("Retreived file: %s\n", arg);
    requestFulfilled++;
    accessTimeSum += time;
    pthread_mutex_unlock(&funcLock); // Unlocks the function
}