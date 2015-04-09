#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int softLink = 0;
	if(argc >= 3){ // Check for switches
		int index;
		for(index = 1; index < argc; index++){
			// enable -s switch
			if(strcmp(argv[index], "-s") == 0){
				softLink = 1;
			}		
		}
	}else {
		perror("Usage: my_ln [optional flag] <original file> <linked file>\n");
		exit(1);
	}

	if(softLink == 0)
		symlink(argv[argc-2], argv[argc-1]);
	else
		link(argv[argc-2], argv[argc-1]);

	return 0;
} 
