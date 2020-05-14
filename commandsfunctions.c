#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashtable.h"
#include "comands.h"
#include "preprocessing.h"
#include "date.h"
#include "list.h"


void listCountries(){

	

}

void global_disease_stats_with_param(LinkedList* list , char *parameter1 , char* parameter2){

	int count = 0;
	int num_of_nodes = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	count = get_node_count(list);

	while(temp->next!=NULL){
		
		//gia kathe komvo sti lista pou einai enas ios metraw posa nodes exei to dentro se sigekrimeno range
		temp= temp->next;
		for (int i = 0; i < count - 1 ; i++)
		{			

			//metraw posa nodes einai mesa sto range pou thelw
			num_of_nodes = find_count_in_range(temp->root->root,parameter1,parameter2);

		}
		//printf("%s has: %d cases \n",temp->entryName,num_of_nodes);
		printf("%s %d\n",temp->entryName,num_of_nodes);
	}
}

void disease_frequency(){



}


void disease_frequency_with_param(){


 }

void searchPatientRecord(){


}

void numPatientAdmissions(){

}

void numPatientAdmissionsParam(){

}


void numPatientDischarges(){


}

void numPatientDischargesParam(){


}



// ------------------------------------------------voithitikes sinartiseis ---------------------------------------------------------	

// nodes of the list
int get_node_count(LinkedList* head) 
{ 
    int count = 0;   
    LinkedList* current = head;
    while (current != NULL) 
    { 
        count++; 
        current = current->next; 
    } 
    return count -1 ; 
} 

//nodes of the tree
int get_tree_nodes_count(avltreenode *root) 
{ 
    int count = 1;
    if (root->left != NULL) {
       count += get_tree_nodes_count(root->left);
    }
    if (root->right != NULL) {
        count += get_tree_nodes_count(root->right);
    }
    return count; 
} 

//nodes of the tree in range [d1,d2] 
int find_count_in_range(avltreenode *root, char* d1, char* d2) 
{ 
    if (root == NULL) return 0; 
  
    // if node is in range include it in count and recur for left and right children of it 
    if (to_seconds(root->data->entryDate) <= to_seconds(d2) && to_seconds(root->data->entryDate) >= to_seconds(d1)) 
        return 1 + find_count_in_range(root->left, d1, d2) + find_count_in_range(root->right, d1, d2); 
  
    // if current node is smaller than d1, then recur for right child 
    else if (to_seconds(root->data->entryDate) < to_seconds(d1)) 
        return find_count_in_range(root->right, d1, d2); 
  
    // else recur for left child 
    else return find_count_in_range(root->left, d1, d2); 
} 


int find_count_in_range_for_country(avltreenode *node, char *date1, char *date2, char *vcountry) {


    // an vrw to country pou thelw kai an d1<= entryDate <= d2 kai d1<= exitDate <= d2 
    int toReturn = 0;
    if((strcmp(node->data->country,vcountry) == 0 && 

		((to_seconds(node->data->entryDate) >= to_seconds(date1)) &&

		(to_seconds(node->data->entryDate) <= to_seconds(date2))) &&

		(to_seconds(node->data->exitDate) >= to_seconds(date1)) &&

		(to_seconds(node->data->exitDate) <= to_seconds(date2)))){


        toReturn = 1;
    } 
    if (node->right == NULL && node->left == NULL)
        return toReturn;


	else if (node->left == NULL)

        return toReturn + find_count_in_range_for_country(node->right, date1, date2, vcountry);

    else if (node->right == NULL)

        return toReturn + find_count_in_range_for_country(node->left, date1, date2, vcountry); 
    
    else
        return toReturn + find_count_in_range_for_country(node->right, date1, date2, vcountry) +

               find_count_in_range_for_country(node->left, date1, date2, vcountry);


}