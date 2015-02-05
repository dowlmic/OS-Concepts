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

   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("Could not attach to new shared memory space\n");
      exit (1);
   }

   struct sh_mem *mem = (void *)shmPtr;
   mem->writeable = 1;
   memset(mem->read_array, 0, 2);
   memset(mem->message, '\0', 25);

   while(strcmp(mem->message, "quit\n") != 0)
   {
       if(mem->writeable == 1){
           fgets(mem->message, 25, stdin);
           mem->writeable = 0;
       }
       
   }


   if (shmdt (shmPtr) < 0) {

      perror ("Can't detach from shared memory\n");
      exit (1);
   }

   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("Can't stuff\n");
      exit(1);
   } 
   return 0;
}


