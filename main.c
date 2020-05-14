#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include "namedpipes.h"
#include "list.h"



int main (int argc,char* argv[]){


	char* input_dir = NULL;
    unsigned long int numWorkers = 0;
    unsigned long int buffsize = 0;
    unsigned long int per_dir = 0;

  	pathsStruct* p = NULL;

   
  //-------------------------------------Parse arguments from command line----------------------------------------------------
    for(int i = 0; i < argc; i++)
    {
        
        // strtoul converts the initial part of the string in str to an unsigned long int value according to the given base
        if(strcmp(argv[i],"-w") == 0)
        {		
            i++;
            numWorkers = strtoul(argv[i], NULL, 10);
        }

        // Get buff size
        else if(strcmp(argv[i],"-b") == 0)
        {
            i++;
            buffsize = strtoul(argv[i], NULL, 10);
        }

        //get input_dir
        else if(strcmp(argv[i],"-i") == 0)
        {
            i++;
            input_dir = malloc(strlen(argv[i]) * sizeof(char) + 5); // for '\0' giati to evgaze i valgrind
            strcpy(input_dir, argv[i]);
        }
    } 

    printf("%ld %ld %s \n",numWorkers,buffsize,input_dir);
    //check if the inputs are valid
    if(!(input_dir != NULL && numWorkers != 0 && buffsize != 0)){

        printf("Wrong inputs: inputs must be like './diseaseAggregator –w numWorkers -b buffsize -i input_dir' \n");  

    } else

        printf("Starting the program with the following inputs: numWorkers: %ld, buffsize: %ld, path: %s \n",numWorkers,buffsize,input_dir);



  //----------------------------------- metraw posa dir exei mesa to input_dir -------------------------------------

    int countryNum = 0; //posa country directories exw
    struct dirent *de;  // Pointer for directory entry
    
    char Input_parent[24];
 	char Output_parent[24];
 	pid_t pids[numWorkers];

 	pid_t P;

    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(input_dir); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // diavazei ta dir
    while ((de = readdir(dr)) != NULL) {

    // den diavazw ta arxeia mesa 	
	if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;

        else {    
            
            countryNum++;
            } 

    }
  
  	printf("Number of directories: %d\n",countryNum);
    closedir(dr);   
//------------------------------------------- diaxwrismos dir stous workers--------------------------------------------------------

    int docsdiv = 0;

    per_dir = countryNum/numWorkers;  // posa paths tha parei o kathenas

    printf("per dir is %ld\n",per_dir);


	if (countryNum%numWorkers != 0){

	  docsdiv= countryNum%numWorkers;  //an perisseuoun

	}

	if(per_dir == 0){
	    printf("Some kids does not take paths (num of kids:)%ld\n",(numWorkers-countryNum));
	    numWorkers = countryNum; // posoi worker tha doulevoun
	}

	printf("numWorkers pou tha doulevoun %ld\n",numWorkers);

//------------------------------------------ ftiaxnw tous workers -------------------------------------------------------------

	char line[buffsize], reply[buffsize]; // in out messages

    //create fifos: 2 fifos gia kathe worker
	createFIFOS(numWorkers);
	sleep(1); 

  	pid_t mypid = getpid();

  	// ftiaxnw tous workers
  	 for (int i = 0; i < numWorkers; i++) {

        P = fork();

        if (P == -1) {
			perror ("Failed to fork") ;
			exit(1) ;
		}		

		//Parent process
        if(P > 0) {
	
		  // printf("parent\n");
          break;
        }


        //child
        else {

        // tha anoigei ton fakelo pou tha parei kai tha anoigei to arxeio kai tha ftiaxnei tis domes

        //CHILD//
        char msgbuf[BUFSIZE];
        int in,out,child_numdocs=numdocs,id=i, linestxt=0;
        int cwords=0, clines=0,cbyte=0;
        if(id < docsdiv){
          child_numdocs++;
        }
        if(child_numdocs==0)return 0;
        char** my_paths;
        my_paths = malloc(sizeof(char*)*child_numdocs);

        //open these pipes
        if((out=open(Input_parent,O_WRONLY))==-1){
          perror("Fail to open the named Output pipe\n");
          exit(1);
        }
        if((in=open(Output_parent,O_RDONLY|O_NONBLOCK))==-1){
            perror("Fail to open the named Input pipe\n");
            exit(1);
        }


        }



        printf("i is : %d .Process has as ID the number : % ld \n",i,(long)getpid());
    	
      }


// -----------------------------------------------------------------------------------------------------------------------

    

//---------------------------------------- start the workes---------------------------------------------------------------

//   	// Start CHILDREN. //
//   for (int i = 0; i < numWorkers; i++) {
//     sprintf(Input_parent, "Input%d", i);
//     sprintf(Output_parent,"Output%d",i);

//     //make these pipes
//     if(mkfifo(Input_parent,0666)==-1){
//         perror("Fail to make named pipe\n");
//         exit(1);}

//     if(mkfifo(Output_parent,0666)==-1){
//         perror("Fail to make named pipe\n");
//         exit(1);}


//     if ((pids[i] = fork()) < 0) {
//       fprintf(stderr, "Wrong in fork\n");
//       exit(0);

//     } else if (pids[i] == 0){

//     	printf("hi\n");
// //--------------------------------------------------------------------------------------------------------------------
// 		}

// 	}




// ----------------- lista -------------------

// LinkedList* countrylist = allocate_list();


// -------------------------------------------



// ////////////////////////////////////////////////////////////////////////////////
//         char *logtxt = malloc(sizeof(char)*strlen("./Log/Worker_.txt")+10);
//         sprintf(logtxt,"./Log/Worker_%d.txt",id);
//         FILE *f3 = fopen(logtxt, "w");
//         if (f3 == NULL){
//           fprintf(stderr,"Error opening file f3\n");
//           exit(-1);
//         }
// ////////////////////////////////////////////////////////////////////////////////


//deleteFIFOS(numWorkers);

}



