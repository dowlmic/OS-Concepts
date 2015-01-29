#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


void* find_file(void* arg);

int main(){
    char* input;
    while(1){
        fgets(input, 50, stdin);
	find_file(input);
    }
}

void* find_file(void* arg){
  
}
