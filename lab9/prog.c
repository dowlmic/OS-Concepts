#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int global1;
int global2;
int global3;
int global4 = 10;
int global5 = 1;
int global6 = 5;

void stuff(){
	printf("Something\n");
}

void stuff2(){
	printf("MOAR Stuff");
}

int main(int argc, char *argv[]){

	int local1;
	int local2;
	int local9;
	int local3 = 2;
	int local4 = 6;
	int local5 = 3;

	int *local6 = malloc(sizeof(int)*20);
	local1 = 4;

	printf("%p\n",&global1);
	printf("%p\n",&global2);
	printf("%p\n",&global3);
	printf("\n");
	printf("%p\n",&global4);
	printf("%p\n",&global5);
	printf("%p\n",&global6);
	printf("\n");
	printf("%p\n",&local1);
	printf("%p\n",&local2);
	printf("%p\n",&local9);
	printf("\n");
	printf("%p\n",&local3);
	printf("%p\n",&local4);
	printf("%p\n",&local5);
	printf("\n");
	printf("%p\n",&local6);
	printf("%p\n", local6);

	int *local7 = malloc(sizeof(int)*20);
	printf("%p\n", &local7);
	printf("%p\n", local7);
	int *local8 = malloc(sizeof(int)*20);
	printf("%p\n", &local8);
	printf("%p\n", local8);
	printf("\n");
	printf("%p\n", &stuff);
	printf("%p\n", &stuff2);
	printf("%p\n", &main);
	printf("\n");
	printf("%p\n", &argc);
	printf("%p\n", &argv);
	printf("%p\n", argv);

	getchar();
	free(local6);
	free(local7);
	free(local8);
	return 0;
}
