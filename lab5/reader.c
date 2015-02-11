/* 
 * reader.c
 *
 * Authors: Michelle Dowling and Lucas Ordaz
 *
 * This program attaches to a specifically designated shared memory location
 * and prints information stored there. Multiple readers are supported.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <pthread.h>
#include "sh_defs.h"

// Mutex to ensure that no two readers write to the shared memory at the same
// time
pthread_mutex_t lock;

// Function that uses the mutex lock to add/subtract to an int*
void changeNumReader(int*, int);

int main(){
   int shmId = -1;
   int *shmPtr;

   // Initialize the mutex lock
   if(pthread_mutex_init(&lock, NULL) != 0) {
      printf("Unable to create mutex");
      exit(1);
   }

   // Get the location of the shared memory from the file
   FILE *f = fopen("shm.txt", "r");
   if (f) {
     fscanf(f, "%d", &shmId);
     fclose(f);

     // Attach process to shared memory location
     if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
       perror ("Could not attach to new shared memory space\n");
       pthread_mutex_destroy(&lock);
       exit (1);
     }

     // Get a pointer to the shared memory
     struct sh_mem *mem = (void *)shmPtr;

     // Flags to determine when the process should read from the shared
     // memory location or stop reading and detach
     int flag = 1;
     int shouldRead = 1;
     while(flag)
     {
         // If we are not writing, then we are reading
         if(!mem->writeable) {

             // Only read if we have not done so yet
             if (shouldRead) {

               // Increment the number of readers reading and print the msg
               changeNumReader(&mem->numReaders, 1);
               printf("%s", mem->message);

               // Exit out of while loop if we get a "quit" message
               if(strcmp("quit\n", mem->message) == 0) {
                 flag = 0;
               }

               // Decrement the number of readers reading
               changeNumReader(&mem->numReaders, -1);

               // If there are no readers reading, tell the writer that it
               // can write
               if(!mem->numReaders) {
                 mem->writeable = 1;
               }

               // Since we have read the data, we should not read it again
               shouldRead = 0;
            }
         }

         // If the writer is writing, then the reader should make sure it
         // can read the new data when it is ready
         else {
           shouldRead = 1;
         }
       
     }

     // Detach from the shared memory location
     if (shmdt (shmPtr) < 0) {
        perror ("Can't detach from shared memory\n");
        pthread_mutex_destroy(&lock);
        exit (1);
     }
   }

   // Make sure the mutex lock is cleaned up too
   pthread_mutex_destroy(&lock);

   return 0;
}

// Method to ensure mutual exclusion when the readers increment/decrement
// values in the shared memory location
void changeNumReader(int *value, int addValue) {
   pthread_mutex_lock(&lock);
   *value = *value + addValue;
   pthread_mutex_unlock(&lock);
}
