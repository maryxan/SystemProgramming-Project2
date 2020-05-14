#ifndef AVLTREE_H
#define AVLTREE_H
 
#include <stdlib.h>


typedef struct
{
	char recordID[100];
	char status[100];
	char patientFirstName[100];
    char patientLastName[100];
    char diseaseID[100];
    char age[100];
}entry;


typedef struct avltreenode
{
    struct avltreenode * left;
    struct avltreenode * right;
    unsigned int height;
    entry* data;
}avltreenode;
 
typedef struct avltree {
    avltreenode * root;
}avltree;
 
 
avltree * create_tree(entry* root);
avltreenode *create_avl_node(entry* item);

avltreenode *insert_to_tree(avltreenode* node, entry* item) ;
avltreenode *right_tree_rotate(avltreenode *y);
avltreenode *left_tree_rotate(avltreenode *x); 

int max(unsigned int  a, unsigned int b);
int get_tree_balance(avltreenode* node);
int get_tree_height(avltreenode* node); 


int get_tree_nodes_count(avltreenode *root);
void avl_tree_free(avltreenode *root);


#endif 