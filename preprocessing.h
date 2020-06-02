#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "avl.h"

int check_record_id(LinkedList* list, char* ID,char* filename);
Stats** preprocessing(char* inputfile,pid_t workerpid,LinkedList* countrylist,LinkedList* diseaselist,char* filename,disease_node *head);



