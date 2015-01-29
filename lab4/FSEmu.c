#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>


void* find_file(void* arg);

int main(){
    pthread_t thread;
    int status;
    char input[50];
    int numRequests = 0;

    // Set up randomness
    srand(time(NULL));

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
    exit(0);
}

void* find_file(void* arg){
    // Simulate a chached file vs a non-cached file
    int chachedFile = rand() % 5;
    if (chachedFile <= 3) {
	sleep(1);
    }
    else {
	int sleepTime = (rand() % 3) + 7;
        sleep(sleepTime);
    }

    // Simulate file retrieval
    printf("Retreived file: %s\n", (char *)arg);  

    return NULL;
}
