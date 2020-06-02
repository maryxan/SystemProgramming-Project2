#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <signal.h>
#include "namedpipes.h"
#include "list.h"
#include "preprocessing.h"
#include "commandsfunctions.h"

int pstrcmp( const void* a, const void* b )
{
  return strcmp( *(const char**)a, *(const char**)b );
}

int main (int argc,char* argv[]){

	char* input_dir = NULL;
    unsigned long int numWorkers = 0;
    unsigned long int buffsize = 0;
    unsigned long int per_dir = 0;
    pathsStruct* p = NULL; 

    LinkedList* countrylist = allocate_list();
    LinkedList* diseaselist = allocate_list();

    char* parameter1;
    char* parameter2;
    char* virus;
    char* vcountry;
    char* ID;
   	char *recid;

	int total_queries = 0;
	int succ = 0;
	int fail = 0;  
  	
  //-------------------------------------Parse arguments from command line----------------------------------------------------

    for(int i = 0; i < argc; i++){
        
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

    //check if the inputs are valid
    if(!(input_dir != NULL && numWorkers != 0 && buffsize != 0)){

        printf("Wrong inputs: inputs must be like './diseaseAggregator –w numWorkers -b buffsize -i input_dir' \n");  

    } else

        printf("Starting the program with the following inputs: numWorkers: %ld, buffsize: %ld, path: %s \n",numWorkers,buffsize,input_dir);

   //----------------------------------------metraw posa dir exei mesa to input_dir------------------------------------------------------

    struct dirent *de;  // Pointer for directory entry
    int countryNum = 0; //posa country directories exw
    int count = 0;



    DIR *dr = opendir(input_dir); //open ./input
	if (dr != NULL)  // opendir returns NULL if couldn't open directory 
    {   	
	    // diavazw ta dir
	    while ((de = readdir(dr)) != NULL) {

	    // den diavazw ta arxeia mesa 	
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
	            continue;

	        else {    
	            
	            countryNum++;

            }
    }
    rewinddir(dr);
   	closedir(dr);  
       
    } else {

    	printf("Could not open current directory\n"); 
        return 0; 
    }
  	
  //------------------------------------------- diaxwrismos dir stous workers--------------------------------------------------------

    int remaining = 0; // posa dir periseuoun

    per_dir = countryNum/numWorkers;  // posa paths tha parei o kathenas

	if (countryNum%numWorkers != 0 || numWorkers == 1){

		remaining = (countryNum - numWorkers); // tha elegxw mono gia >0
	}

	if(per_dir == 0){
	    printf("Some workers don't take paths (num of workers:)%ld\n",(numWorkers-countryNum));
	    numWorkers = countryNum; // posoi worker tha doulevoun
	    per_dir = 1;
	}

//------------------------------------------ ftiaxnw tous workers -------------------------------------------------------------

    pid_t childpid;
 	pid_t parent = getpid(); // to ID tou gonea 
 	pid_t workers[numWorkers];


	pid_t child_pid;

	for(int i=0;i<numWorkers;i++){
		workers[i] = 0;
	}

	//-------------- create fifos -----------------------
	createFIFOS(numWorkers);
	sleep(1);
	//-------------- create the workers -----------------
	for(int i = 0; i < numWorkers; i++){

		childpid = fork();

		if (childpid == -1) {
			perror("Unsuccessful fork\n");
			exit(1) ;
		} 
		//child
		if (childpid == 0){

			workers[i] = getpid();    		
		} 

		//parent process
		if(childpid > 0){

			break;
		}	
	//printf("i is : %d .Process has as ID the number : % ld with parent id : %ld \n",i,(long)getpid(),(long)parent);
	}


	//if parent
	if(parent == getpid()){

		//open named pipes and read from stdin		
		struct dirent *de;
		DIR *dr = opendir(input_dir); //open ./input


		//making an array of paths and fill it with the directories
	   	pathsStruct* p = malloc(sizeof(pathsStruct)); 
		p->paths = malloc(countryNum*sizeof(char*));
		p->noOfPaths = countryNum;
		int i = 0;


	//---------------------------------------------------------------------------------------------------------
		//anoigw ton ./input_dir kai ftiaxnw ton pinaka me ta paths

		while ((de = readdir(dr)) != NULL) {

		    // den diavazw ta arxeia mesa 	
			if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0 || de->d_ino == 0)
		            continue;

		    else {    
		            
			char* newname = malloc(strlen(input_dir)+strlen(de->d_name)+2);
			strcpy(newname, input_dir); // Compose pathname as "name/de->d_name"
			strcat(newname, "/");
			strcat(newname, de->d_name);


			p->paths[i] = malloc((strlen(de->d_name)+1)*sizeof(char));
			strcpy(p->paths[i],newname);
			//printf("path is %s\n",p->paths[i]);
			i++;


			free(newname);


		    } 

		}
	//---------------------------------tha moirasw ta paths----------------------------------------------

		//stelnw ta paths me methodo round robin
		char msg[buffsize];

		int* readfds = malloc(numWorkers*sizeof(int));
		int* writefds = malloc(numWorkers*sizeof(int));
		int* folders = malloc(numWorkers*sizeof(int));
		int folders_i;

		for( folders_i=0 ; folders_i<numWorkers ; folders_i++){
			folders[folders_i]=0;
		}
		int j = 0;
		for (int k = 0; k < numWorkers; k++){


			char FIFO1[24]; // to read
	  		char FIFO2[24]; // to write

	  		// anoigw ta pipe gia ton kathe worker
	  		sprintf(FIFO1,"Input%d",k);
	   		sprintf(FIFO2,"Output%d",k);

			int fd2 = -1;

			int nwrite;

			int readfd, writefd;

		
			if((readfd = open(FIFO1, O_RDONLY)) < 0) {
				perror("server: can't open read fifo");
			}
			if((writefd = open(FIFO2, O_WRONLY)) < 0) {
				perror("server: can't open write fifo");
			}

			fcntl(writefd, F_SETFL, O_NONBLOCK);
			
			readfds[k] = readfd;
			writefds[k] = writefd;


			strcpy(msg,p->paths[j]);
		 	//printf("Parent is sending the path: %s\n",msg);
			folders[k]++;
			if((nwrite = write(writefds[k],msg,buffsize)) == -1){

				perror ( " Error in Writing " );
			}

			j++;
		

		}

		// an uparxoyn akoma monopatia enw exw kanei tin prwti moirasia
		sleep(1);
		if(remaining > 0){

			int rem = p->noOfPaths - remaining;

			for (int i = 0; i < remaining; i++)
			{
				
				strcpy(msg,p->paths[rem]);
			 	//printf("Parent is sending the path: %s\n",msg);


			 	int nwrite;
		  		if(numWorkers == 1){

		  			folders[0]++;
					if((nwrite = write(writefds[0],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}
		  		}
		  		else{

		  			folders[i%numWorkers]++;
					if((nwrite = write(writefds[i%numWorkers],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}
			   	}


				rem++;

			}


		}
	
		//diavazw ta statistics apo tous workers
		Stats *get_stats = malloc(sizeof(Stats));
		for (int i = 0; i < numWorkers; ++i){

			int nread,print_folder=1;
			char prev_date[100]="";
			do {
				if((nread = read(readfds[i],get_stats,sizeof(Stats))) < 0){
					perror ( " Error in Reading " );
				}
				if(print_folder==1){
					print_folder = 0;
					printf("%s\n",get_stats->countryName);
				}
				if(strcmp(get_stats->diseaseID,"ENDOFFOLDER")==0){
					print_folder = 1;
					folders[i]--;
					continue;
				}
				if(strcmp(prev_date,get_stats->date)!=0){
					printf("%s\n",get_stats->date);
					strcpy(prev_date,get_stats->date);
				}
				printf("%s\n",get_stats->diseaseID);
				printf("Age range 0-20 years: %d cases\n",get_stats->range1);
				printf("Age range 21-40 years: %d cases\n",get_stats->range2);
				printf("Age range 41-60 years: %d cases\n",get_stats->range3);
				printf("Age range 60+ years: %d cases\n",get_stats->range4);
			}while (folders[i]>0);

		}
		//free(get_stats);
		//--------------------------------- afou stalthoun oi katalogoi tha perimenei input --------------------------	
	    	
		char* command;
		size_t len = 0;
		char* line = NULL;
		char* l = NULL;

		 while(getline(&line,&len,stdin)){

		 	l = strtok(line,"\n");
			if(l == NULL){
				continue;
			}

			char copy[100];
		 	char *str = malloc(strlen(l) + 1);

		 	strcpy(str,l);
			command = strtok(str," ");

			//printf("%s\n",command);

			//works
			if(strcmp(command,"/exit")==0){

				printf("exiting\n");
				total_queries++;
				succ++;

				for (int i = 0; i < numWorkers; ++i)
				{

					strcpy(msg,command);

				 	int nwrite;

					if((nwrite = write(writefds[i],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}



				}
				for (int i = 0; i < numWorkers; ++i){

					memset(msg, '\0', sizeof (msg));
					int nread;
					//printf("READ\n");
					if((nread = read(readfds[i],msg,sizeof(msg))) < 0){

						perror ( " Error in Reading " );
					}
				}

				break;
			}

			//works
			if(strcmp(command,"/listCountries") == 0)
			{
				total_queries++;
				succ++;

				for (int i = 0; i < numWorkers; ++i)
				{

					strcpy(msg,command);

				 	int nwrite;

					if((nwrite = write(writefds[i],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}



				}

				
			}

			// /diseaseFrequency virusName date1 date2 [country]
	        else if(strcmp(command,"/diseaseFrequency") == 0) //
	        {
	        	total_queries++;
	        	int sum = 0;

	            for (int i = 0; i < numWorkers; i++)
				{

					strcpy(msg,l);

				 	int nwrite;

					if((nwrite = write(writefds[i],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}

				}
				for (int i = 0; i < numWorkers; ++i){

					memset(msg, '\0', sizeof (msg));
					int nread;
					//printf("READ\n");
					if((nread = read(readfds[i],msg,sizeof(msg))) < 0){

						perror ( " Error in Reading " );
					}
					//printf("%s\n", msg);
					if(atoi(msg) == 0){
						fail++;
					}
					sum += atoi(msg);
				}

				printf("%d\n",sum );

	        }

	        //works for print ID-NAME-LASTNAME-DISEASE-AGE
	        // /searchPatientRecord recordID
	        else if(strcmp(command,"/searchPatientRecord") == 0) 
	        {

	        	total_queries++;
	        	succ++;
	        	for (int i = 0; i < numWorkers; i++)
				{

					strcpy(msg,l);

				 	int nwrite;

					if((nwrite = write(writefds[i],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}

				}

				for (int i = 0; i < numWorkers; ++i){


					memset(msg, '\0', sizeof (msg));
					int nread;
					//printf("READ\n");
					if((nread = read(readfds[i],msg,sizeof(msg))) < 0){

						perror ( " Error in Reading " );
					}

					fail += atoi(msg);
				}


	        }

	        // /numPatientAdmissions disease date1 date2 [country]
	        else if(strcmp(command, "/numPatientAdmissions") == 0) 
	        {
	           	total_queries++;
	           	succ++;	
	            int count = 0;	

	        	for (int i = 0; i < numWorkers; i++)
				{

					strcpy(msg,l);

				 	int nwrite;

					if((nwrite = write(writefds[i],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}

				}

				for (int i = 0; i < numWorkers; ++i){


					memset(msg, '\0', sizeof (msg));
					int nread;
					//printf("READ\n");
					if((nread = read(readfds[i],msg,sizeof(msg))) < 0){

						perror ( " Error in Reading " );
					}
					if(atoi(msg) == 0){
						fail++;
					}
					count += atoi(msg);

				}

				printf("%d\n",count );

	        }
	        // /numPatientDischarges disease date1 date2 [country]
	        else if(strcmp(command, "/numPatientDischarges") == 0) 
	        {
				total_queries++;
				succ++;	            
	        	int count = 0;
	        	for (int i = 0; i < numWorkers; i++)
				{

					strcpy(msg,l);

				 	int nwrite;

					if((nwrite = write(writefds[i],msg,buffsize)) == -1){

						perror ( " Error in Writing " );
					}

				}
				for (int i = 0; i < numWorkers; ++i){


					memset(msg, '\0', sizeof (msg));
					int nread;
					//printf("READ\n");
					if((nread = read(readfds[i],msg,sizeof(msg))) < 0){

						perror ( " Error in Reading " );
					}

					if(atoi(msg) == 0){
						fail++;
					}
					count += atoi(msg);

				}

				printf("%d\n",count );

	        }

			else {

				printf("command not found\n");
				fail++;
				total_queries++;
			}	

			free(str);
		}
		free(line);

		deleteFIFOS(numWorkers);

		//------------- ftiaxnw to log file -----------------------
		FILE *fp;
		char filename[20];
		sprintf(filename, "./output/log_file.%ld",(long)parent);
		fp = fopen(filename,"w");

		if(fp == NULL){
		    /* File not created hence exit */
		    printf("Unable to create file.\n");
		    exit(EXIT_FAILURE);
		}

		for (int i = 0; i < p->noOfPaths; ++i){
        	fputs(p->paths[i]+8, fp);
        	fputs("\n",fp);
		}
		fputs("TOTAL:",fp);
		fprintf(fp,"%d",total_queries);
		fputs("\n",fp);

		fputs("SUCCESS:",fp);
		fprintf(fp,"%d",succ);
		fputs("\n",fp);

		fputs("FAIL:",fp);
		fprintf(fp,"%d",fail);

		fclose(fp);

		
		destroyPathsStruct(&p);

		for(int i=0;i<numWorkers;i++){
			close(readfds[i]);
			close(writefds[i]);
		}
		
		free(readfds);
		free(writefds);
		free(folders);
		//---------------SIGKILL TO WORKERS-----------------------
		for (int i = 0; i < numWorkers; i++){

		    kill(workers[i],SIGKILL);
		}

	} // end of parent 

		//if child 
		else {

			char buf[buffsize];
			int res;

			int pos = returnPosWorker(numWorkers,getpid(),workers);
		
					
			char FIFO1[24]; // to read
			char FIFO2[24]; // to write

			sprintf(FIFO1,"Input%d", pos);
			sprintf(FIFO2,"Output%d",pos);

					
			int readfd, writefd;

			// Open the FIFOs
			if((writefd = open(FIFO1, O_WRONLY)) < 0)
			{
				perror("client: can't open write fifo \n");
			}
			fcntl(writefd, F_SETFL, O_NONBLOCK);

			if((readfd = open(FIFO2, O_RDONLY)) < 0)
			{
				perror("client: can't open read fifo \n");
			}	

			while(1){


					char cpy[100];
					char cpy2[100];
					char cpy3[100];
					char cpy4[100];

				 	memset(buf, '\0', sizeof (buf));

					if ((res = read(readfd,buf,buffsize) )< 0) {

					perror ( "problem in reading \n" ) ;
						break;

					}
					else if (res == 0)
	           			break;
				
				  /////////////////////////////////////////////////////////////////

	           		strcpy(cpy,buf);
	           		strcpy(cpy2,buf);
	           		strcpy(cpy3,buf);
	           		strcpy(cpy4,buf);

	           		//works
					if(strcmp(buf,"/listCountries") == 0){

						succ++;

						listCountries(countrylist);
						
					}

					//works for id-name-surname-disease-age 
					else if(strcmp(strtok(buf," "),"/searchPatientRecord") == 0) 
			        {	

			        	int final = 0;
			        	char m[100];
			        	recid = strtok(NULL," ");

						succ++;

			        	//printf("%s\n",recid);
			            final += searchPatientRecord(countrylist,recid);

			            //printf("%d\n",final);
			            sprintf(m, "%d", final);

			            if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
						}	

			        }

			        // /diseaseFrequency virusName date1 date2 [country]
			        else if(strcmp(strtok(cpy," "),"/diseaseFrequency") == 0){

			        	int final = 0;
			        	char m[100];

			        	virus = strtok(NULL," ");
			            parameter1 = strtok(NULL," "); 
			            parameter2 = strtok(NULL," ");
			            vcountry = strtok(NULL," ");

			 
			            if (vcountry == NULL)
			            {	

			            	succ++;
			            	final += disease_frequency(diseaselist,virus,parameter1,parameter2);

			            	sprintf(m, "%d", final);
			            	//printf("FINAL %d\n",final);

			            	if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
							}
			            }
			            else {

			            	succ++;
			            	final += disease_frequency_with_param(diseaselist,virus,parameter1,parameter2,vcountry);
			            	sprintf(m, "%d", final);

			            	if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
							}
			            }

			        }
					// /numPatientAdmissions disease date1 date2 [country]
			        else if(strcmp(strtok(cpy2," "),"/numPatientAdmissions") == 0) 
			        {
			        	int final = 0;
			        	char m[100];

			            virus = strtok(NULL," ");
			            parameter1 = strtok(NULL," "); 
			            parameter2 = strtok(NULL," ");
			            vcountry = strtok(NULL," ");


			           // printf("%s %s %s %s\n",virus,parameter1,parameter2,vcountry );
			            if (vcountry == NULL)
			            {
			             	succ++;
			            	final += numPatientAdmissions(diseaselist,virus,parameter1,parameter2);

			            	sprintf(m, "%d", final);

			            	if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
							}

			            } else {
			   				
			   				succ++;
			                final += numPatientAdmissionsParam(diseaselist,virus,parameter1,parameter2,vcountry);
			                sprintf(m, "%d", final);

			            	if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
							}
			            }

			        }
			        // /numPatientDischarges disease date1 date2 [country]
			        else if(strcmp(strtok(cpy3," "), "/numPatientDischarges") == 0) 
			        {

			        	int final = 0;
			        	char m[100];

			            virus = strtok(NULL," ");
			            parameter1 = strtok(NULL," "); 
			            parameter2 = strtok(NULL," ");
			            vcountry = strtok(NULL," ");


			            if (vcountry == NULL)
			            {
			             	succ++;
			            	final +=numPatientDischarges(diseaselist,virus,parameter1,parameter2);
			            	sprintf(m, "%d", final);

			            	if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
							}

			            } else {

			            	succ++;
			           		final += numPatientDischargesParam(diseaselist,virus,parameter1,parameter2,vcountry);
			           		sprintf(m, "%d", final);

			            	if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
							}
			             
			            }

			        }
			        else if(strcmp(strtok(cpy4," "),"/exit") == 0){

			        	char m[100];
			        	strcpy(m,"ok");

			        	succ++;
			        	free_linkedlist(diseaselist);
			         	free_linkedlist(countrylist);

			            // break;
			         	if ((res= write(writefd,m,sizeof(m))) == 0) {

							perror ( "problem in writing \n" ) ;
						}

			        }   
			        
					else{

				  ////////////////////////////////////////////////////////////////////	
						// FEED DATA //

						int files = 0;
						int infiles = 0;
						// metraw posa arxeia exei mesa o fakelos pou phre o worker
						struct dirent *de;
						DIR *dr = opendir(buf); //open ./input
						while ((de = readdir(dr)) != NULL) {


							if((de->d_type == DT_REG)){

								files++;

							}
						
						}

						// ftiaxnw ena pinaka me ta onomata twn arxeiwn
						char** fileNames = malloc(files*sizeof(char*));
						//printf("%d\n",files);

						rewinddir(dr);
						int k=0;
						while((de =readdir(dr)) != NULL && k<files){
							if((de->d_type == DT_REG)){
								fileNames[k] = malloc((strlen(de->d_name)+1)*sizeof(char));
								strcpy(fileNames[k],de->d_name);
								k++;

							}
						}


						//tha kanw sort ton pinaka me ta files

						//void qsort (void* base, size_t num, size_t size,int (*comparator)(const void*,const void*))

						int size = sizeof(fileNames) / sizeof(fileNames[0]); 

						qsort(fileNames, size, sizeof(fileNames[0]), pstrcmp);

						disease_node *head_disease = malloc(sizeof(disease_node));
						head_disease->stats = malloc(sizeof(Stats));
						strcpy(head_disease->stats->diseaseID,"HEAD");
						head_disease->next = NULL; 
						int dis;

						// gia osa arxeia uparxoyn tha ta anoigw 				
						FILE *fp;
						char* line = NULL;
		    			size_t len = 0;

		    			// for every file - open file
		    			for (int i = 0; i < files; i++)	{

		    				char m[buffsize];
		    				char chosen[100];
							strcpy(chosen,fileNames[i]);

		    				char *in = malloc(strlen(buf) + strlen(chosen) + 2);
		    				*in = '\0';

		    				// to kanw ws eksis : ./input_dir/Country/file.txt
		    				strcat(in,buf);
			    			strcat(in,"/");
			    			strcat(in,chosen);

			    			//printf("we are opening the file: %s\n",in);
			    			//feed data into structures
						    preprocessing(in,getpid(),countrylist,diseaselist,fileNames[i],head_disease);
						    free(in);

						    //stats
						    disease_node *temp_dis = head_disease->next;

							while (temp_dis!=NULL){
								strcpy(temp_dis->stats->countryName,buf);
								strcpy(temp_dis->stats->date,chosen);
								if (res= write(writefd,temp_dis->stats,sizeof(Stats) ) == 0) {
									perror ( "problem in writing \n" ) ;
								}
								temp_dis=temp_dis->next;
							}
							disease_node *temp_dis2 = NULL;
							temp_dis = head_disease->next;
							
							// while (temp_dis!=NULL){
							// 	temp_dis2=temp_dis;
							// 	temp_dis=temp_dis->next;
							// 	free(temp_dis2);
							// }
							
						 	head_disease->next = NULL;



						} 
						// free(head_disease->stats);
						// free(head_disease);

						Stats *eof_stats = malloc(sizeof(Stats)) ;
						strcpy(eof_stats->diseaseID,"ENDOFFOLDER");
						if (res= write(writefd,eof_stats,sizeof(Stats) ) == 0) {
							perror ( "problem in writing \n" ) ;
						}

						free(eof_stats);

						//free fileNames array
						for(int k=0;k<files;k++){
							free(fileNames[k]);
							fileNames[k] = NULL;
						}
						free(fileNames);
						fileNames = NULL;	
						closedir(dr);


		    			}
		    		
					}
					
			}	

}



