#ifndef _NAMEDPIPES_H_
#define _NAMEDPIPES_H_

typedef struct pathsStruct{
	int noOfPaths;
	char** paths;
}pathsStruct;

typedef struct
{
	char recordID[100];
	char patientStatus[100];
	char patientFirstName[100];
    char patientLastName[100];
    char diseaseID[100];
    char age[100];
    pid_t workerpid;
    char countryname[100];
    char entrydate[100];
    char exitdate[100];
    char filename[100];
    char stat[100];

}entry;


typedef struct Stats{

	char date[100];
	char countryName[100];
	char diseaseID[100];
	int range1;
	int range2;
	int range3;
	int range4;
	
}Stats;

typedef struct disease_node{

	struct Stats *stats;
	struct disease_node *next;
	
}disease_node;

void createFIFOS(int w);
void deleteFIFOS(int w);

int returnPosWorker(int w,pid_t worker,pid_t* workers);
void destroyPathsStruct(pathsStruct** p);


#endif