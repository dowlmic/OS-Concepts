#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/sem.h>
#define SIZE 16

int main (int argc, char *argv[])
{
   int status;
   long int i, loop, temp, *shmPtr;
   int shmId, semId;
   struct sembuf sbufLock, sbufUnlock;
   pid_t pid;

      // get value of loop variable (from command-line argument)
   loop = atoi(argv[1]);
   semId = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
   if (semId  <= 0){
      perror("Error setting up semaphore");
      exit(1);
   }
   printf("%d\n", semId);
   if ((semctl(semId, 0, SETVAL, 1)) < 0) {
      perror("Error instantiating semaphore");
      exit(1);	
   }

   sbufLock.sem_num = 0;
   sbufLock.sem_op = -1;
   sbufLock.sem_flg = 0;

   sbufUnlock.sem_num = 0;
   sbufUnlock.sem_op = 1;
   sbufUnlock.sem_flg = 0;

   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   shmPtr[0] = 0;
   shmPtr[1] = 1;

   if (!(pid = fork())) {
      for (i=0; i<loop; i++) {
               semop(semId, &sbufLock, 1);
               // swap the contents of shmPtr[0] and shmPtr[1]
               temp = shmPtr[0];
               shmPtr[0] = shmPtr[1];
               shmPtr[1] = temp;
               semop(semId, &sbufUnlock, 1);
      }
      if (shmdt (shmPtr) < 0) {
         perror ("just can't let go\n");
         exit (1);
      }
      exit(0);
   }
   else
      for (i=0; i<loop; i++) {
               semop(semId, &sbufLock, 1);
               // swap the contents of shmPtr[1] and shmPtr[0]
               temp = shmPtr[0];
               shmPtr[0] = shmPtr[1];
               shmPtr[1] = temp;
               semop(semId, &sbufUnlock, 1);
      }

   wait (&status);
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }
   semctl(semId, 0, IPC_RMID);


   return 0;
} 
