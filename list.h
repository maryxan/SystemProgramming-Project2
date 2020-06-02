#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "avl.h"

#ifndef LIST_H
#define LIST_H

typedef struct LinkedList LinkedList;


struct LinkedList {
    char entryName[100];
    pid_t pid;
    LinkedList* next;
    entry* data;
	avltree* root;	

};



LinkedList* allocate_list();
void linkedlist_insert(LinkedList* list, entry* ref,int isCountry);

void free_linkedlist(LinkedList* list);

void print(LinkedList* list);

#endif