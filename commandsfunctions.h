#include "list.h"


#ifndef COMANDSFUNCTIONS_H
#define COMANDSFUNCTIONS_H

void listCountries();
int get_node_count(LinkedList* head); 

int searchPatientRecord(LinkedList *list , char* ID);

int disease_frequency(LinkedList* list ,char *virus ,char *parameter1 , char* parameter2);
int disease_frequency_with_param(LinkedList* diseaselist,char *virus ,char *parameter1 , char* parameter2 , char* vcountry);

int numPatientAdmissions(LinkedList* list,char *virus ,char *parameter1 , char* parameter2);
int numPatientAdmissionsParam(LinkedList* list,char *virus ,char *parameter1 , char* parameter2 , char* vcountry);

int numPatientDischarges(LinkedList* list,char *virus ,char *parameter1 , char* parameter2);
int numPatientDischargesParam(LinkedList* list,char *virus ,char *parameter1 , char* parameter2 , char* vcountry);



int find_count_in_range(avltreenode *root, char* d1, char* d2) ;
int find_count_in_range_for_country(avltreenode *node, char *date1, char *date2, char *vcountry);

int find_count_in_range_for_stats(avltreenode *node, char *date1, char *date2);

int find_count_in_range_for_stats_exit(avltreenode *node, char *date1, char *date2);

int count_tree(avltreenode *node);
#endif