#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int childCount;
int *children;
int parent;

// Function that will contain how to handle each signal
void EventHandler(int signal, siginfo_t *info, void *ptr)
{
    int pid = getpid();
    if(signal == SIGINT && pid == parent) // If the signal is SIGINT terminate both child processes
    {
        printf(" received. Initiating graceful shutdown\n");
        int x;
        for(x = 0; x < childCount; x++)
        {
            printf("Terminating Child %d ----- %d/%d\n", children[x], x+1, childCount);
            fflush(stdout);
            kill(children[x], SIGKILL);
        }
        exit(0);
    }
    else
    {
        switch(signal)
        {
            case SIGUSR1:
                printf("%lu generated signal SIGUSR1\n", (unsigned long)info->si_pid); // Print the signal received
                break;
            case SIGUSR2:
                printf("%lu generated signal SIGUSR2\n", (unsigned long)info->si_pid); // Print the signal received
                break;
            case SIGINT:
                printf("%d generated signal SIGINT, Killing Process\n", pid); // Children received SIGINT
                exit(0);
                break;
            case SIGKILL:
                printf("%lu generated signal SIGKILL\n", (unsigned long)info->si_pid); // Print the signal received
                break;
            case SIGCHLD:
                printf("%lu generated signal SIGCHLD\n", (unsigned long)info->si_pid); // Print the signal received
                break;
            default:
                printf("%lu generated signal unknown signal %d\n", (unsigned long)info->si_pid, signal); // Print the signal received
        }
    }
    fflush(stdout);
}

// What the child runs after a fork
void ChildRunProcess(int parent, int pid)
{
    printf("Child Created %d\n", pid);
    srand(time(NULL));
    int r;
    int sleepTime;
    while(1) {
        r = rand() % 2; // Generates a random number
        sleepTime = (rand() % 4)+1; // Generates sleep time
        fflush(stdout);
        if(r == 0)
            kill(parent,SIGUSR1);
        else
            kill(parent,SIGUSR2);
        fflush(stdout);
        sleep(sleepTime);
    }
}

int main(int argc, char *argv[])
{
    struct sigaction sig;

    // Global assign
    childCount = (argv[1] == NULL) ? 2 : atoi(argv[1]);
    children = realloc(NULL, childCount * sizeof(pid_t));

    sig.sa_sigaction = EventHandler; // Set the function to call on signal
    sig.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &sig, NULL) < 0) // Register SIGUSR1
        printf("Could not register event for SIGUSR1");
    if(sigaction(SIGUSR2, &sig, NULL) < 0) // Register SIGUSR2
        printf("Could not register event for SIGUSR2");
    if(sigaction(SIGCHLD, &sig, NULL) < 0) // Register SIGCHILD
        printf("Could not register event for SIGCHILD");
    if(sigaction(SIGINT, &sig, NULL) < 0) // Register SIGINT
        printf("Could not register event for SIGINT");

    parent = getpid();

    // Create children
    int x;
    for(x = 0; x < childCount; x++)
    {
        pid_t pid;
        if ((pid = fork()) < 0) // Fork
        {
            perror("fork failure");
            exit(1);
        }
        else if (pid == 0) // Child
        {
            ChildRunProcess(parent, getpid());
            exit(0);
        }
        else // Parent
        {
            printf("Created %d\n", pid);
            children[x] = pid;
            fflush(stdout);
        }
    }

    while(1);
    return 0;
}
