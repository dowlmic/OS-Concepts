/*
Authors: Michelle Dowling and Lucas Ordaz
Date: 4/2/2015
*/
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
int main(int argc, char** argv)
{
	DIR *dirPtr;
	struct dirent *entryPtr;
	struct stat statBuf;
	// Flag for printing Size
	int printSize = 0;
	// Flag for printing Inode
	int printInode = 0;
	// Search directory (defaults to local directory)
	char* searchDir = ".";
	
	if(argc >= 2){ // Check for switches
		int index;
		for(index = 1; index < argc; index++){
			// enable -s switch
			if(strcmp(argv[index], "-s") == 0){

				printSize = 1;
			}
			// enable -i switch
			if(strcmp(argv[index], "-i") == 0){

				printInode = 1;
			}
			// Override default directory (if provided)
			if(argv[index][0] != '-'){
				searchDir = argv[index];
			}
		
		}
	}

	// Open the directory
	dirPtr = opendir (searchDir);

	// Go through each tuple
	while ((entryPtr = readdir (dirPtr))){
		// Get the status
		lstat(entryPtr->d_name, &statBuf);
		if(strcmp(entryPtr->d_name, ".") != 0 
			&& strcmp(entryPtr->d_name, "..") != 0){
			// Print File Name
			printf ("%s\t", entryPtr->d_name);
			// Print file size in blocks
			if(printSize == 1)
				printf("Size: %zdB\t",
					statBuf.st_size);
			// Print file inode
			if(printInode == 1)
				printf("Inode: %zd\t", entryPtr->d_ino);
			printf("\n");
		}
	}
	// Close File
	closedir (dirPtr);
	return 0;
}
