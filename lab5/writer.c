/*
 * writer.c
 *
 * Authors: Michelle Dowling and Lucas Ordaz
 *
 * This program creates a shared memory location to write data to for readers
 * to read and return control to the writer
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "sh_defs.h"

int main(){
   int shmId;
   int *shmPtr;

   // Create a shared memory location
   if ((shmId = shmget (IPC_PRIVATE, sizeof(struct sh_mem),
    IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("Error creating new shared memory space.\n");
      exit (1);
   }

   // Attach to the shared memory location
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("Could not attach to new shared memory space\n");
      exit (1);
   }

   // Write the address to the shared memory location to a file
   FILE *f = fopen("shm.txt", "w");
   if (f == NULL) {
      printf("Error opening file\n");
      exit(1);
   }
   else {
      fprintf(f, "%d", shmId);
      fclose(f);

      // Initialize the struct in the shared memory location
      struct sh_mem *mem = (void *)shmPtr;
      mem->writeable = 1;
      mem->numReaders = 0;
      memset(mem->message, '\0', 25);

      // Continuously read user input and store it in the shared memory location
      while(strcmp(mem->message, "quit\n") != 0)
      {
          // Ony write if we are allowed to do so
          if(mem->writeable == 1){
              fgets(mem->message, 25, stdin);

              // Tell the readers that they can read new data
              mem->writeable = 0;
          }
       
      }

      // Detach from the shared memory location
      if (shmdt (shmPtr) < 0) {
         perror ("Can't detach from shared memory\n");
         exit (1);
      }

      // Delete the shared memory
      if (shmctl (shmId, IPC_RMID, 0) < 0) {
         perror ("Can't stuff\n");
         exit(1);
      } 

      // Delete the file with the address for the shared memory location
      remove("shm.txt");

      return 0;
   }
}


