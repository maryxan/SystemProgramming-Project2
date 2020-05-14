#ifndef _NAMEDPIPES_H_
#define _NAMEDPIPES_H_

typedef struct pathsStruct{
	int noOfPaths;
	char** paths;
}pathsStruct;


void createFIFOS(int w);
void deleteFIFOS(int w);


#endif