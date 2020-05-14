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
#define PERMS S_IRUSR | S_IWUSR


//create FIFOS for all workers
void createFIFOS(int w){
	for(int i = 0; i < w; i++){

		char FIFO1[24]; // to read
  		char FIFO2[24]; // to write



	    sprintf(FIFO1,"Input%d", i);
	    sprintf(FIFO2,"Output%d",i);


	    //create fifos for read and writing
		if((mkfifo(FIFO1, PERMS) < 0) && (errno != EEXIST) ) {
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

	for(int i=0; i< w; i++){

		char FIFO1[24]; // to read
  		char FIFO2[24]; // to write



	    sprintf(FIFO1,"Input%d", i);
	    sprintf(FIFO2,"Output%d",i);

		//delete FIFOS
		if(unlink(FIFO1) < 0) {
			perror("client: can't unlink \n");
		}
		if(unlink(FIFO2) < 0) {
			perror("client: can't unlink \n");
		}
		
	}
}

//open FIFOS of parent
void parentFIFOS(int w,char* line){
	int* readfds = malloc(w*sizeof(int));
	int* writefds = malloc(w*sizeof(int));

	for(int i=0;i<w;i++){
		
		char FIFO1[24]; // to read
  		char FIFO2[24]; // to write



	    sprintf(FIFO1,"Input%d", i);
	    sprintf(FIFO2,"Output%d",i);
		
		int readfd, writefd;
		
		if((readfd = open(FIFO1, O_RDONLY)) < 0) {
			perror("server: can't open read fifo");
		}
		if((writefd = open(FIFO2, O_WRONLY)) < 0) {
			perror("server: can't open write fifo");
		}
		
		readfds[i] = readfd;
		writefds[i] = writefd;
		
		free(FIFO1);
		FIFO1 = NULL;
		free(FIFO2);
		FIFO2 = NULL;
	}
	
	//serverSide(readfds, writefds,line,w);
	
	for(int i=0;i<w;i++){
		close(readfds[i]);
		close(writefds[i]);
	}
	
	free(readfds);
	readfds = NULL;
	free(writefds);
	writefds = NULL;
}


//open FIFOS of child
int childFIFOS(int worker,indexesArray* indexesArr,rootNode* root,int logfd){	

	
	
	char FIFO1[24]; // to read
  	char FIFO2[24]; // to write



	sprintf(FIFO1,"Input%d", i);
	sprintf(FIFO2,"Output%d",i);
	
	int readfd, writefd;
	/* Open the FIFOs. We assume server has
	already created them. */
	if((writefd = open(FIFO1, O_WRONLY)) < 0)
	{
		perror("client: can't open write fifo \n");
	}
	if((readfd = open(FIFO2, O_RDONLY)) < 0)
	{
		perror("client: can't open read fifo \n");
	}
	
	//int ret = clientSide(readfd,writefd,indexesArr,root,logfd);
	close(readfd);
	close(writefd);
	
	
	return ret;
}