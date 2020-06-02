#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/errno.h>

#include "namedpipes.h"
#define PERMS 0777

//create FIFOS for all workers
void createFIFOS(int w){

	char FIFO1[100]; // to read
  	char FIFO2[100]; // to write

	for(int i = 0; i < w; i++){

	    sprintf(FIFO1,"Input%d", i);
	    sprintf(FIFO2,"Output%d",i);

	    //create fifos for read and writing
		if((mkfifo(FIFO1, PERMS) < 0) && (errno != EEXIST)) {
			perror("can't create fifo");
		}
		if((mkfifo(FIFO2, PERMS) < 0) && (errno != EEXIST)) {
			unlink(FIFO1);
			perror("can't create fifo");
		}
	}
}


//delete FIFOS
void deleteFIFOS(int w){

	char FIFO1[100]; // to read
  	char FIFO2[100]; // to write

	for(int i=0; i< w; i++){
		
	    sprintf(FIFO1,"Input%d", i);
	    sprintf(FIFO2,"Output%d",i);

		//delete FIFOS
		if(remove(FIFO1) != 0 && FIFO1 != NULL) {
			perror("client: can't unlink \n");
		}
		if(remove(FIFO2) != 0 && FIFO2 != NULL ) {
			perror("client: can't unlink \n");
		}
		
	}
}

int returnPosWorker(int w,pid_t worker,pid_t* workers){
	
	for(int i=0;i<w;i++){
		if(workers[i]==worker){
			return i;
		}
	}
	return -1;
}

void destroyPathsStruct(pathsStruct** p){

	for(int i=0;i<(*p)->noOfPaths;i++){
		free((*p)->paths[i]);
		(*p)->paths[i] = NULL;
	}
	free((*p)->paths);
	(*p)->paths = NULL;
	free((*p));
	*p = NULL;
}