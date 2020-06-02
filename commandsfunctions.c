#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "preprocessing.h"
#include "commandsfunctions.h"
#include "list.h"
#include "date.h"

void listCountries(LinkedList *list){

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	temp= temp->next;
	while(temp!=NULL){
		
		printf("%s %ld\n",temp->entryName ,(long)temp->pid);
		fflush(stdout);
		temp= temp->next;
	}
}


int disease_frequency(LinkedList* list ,char *virus ,char *parameter1 , char* parameter2){


	int count = 0;
	int num = 0;
	int num_of_nodes = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	count = get_node_count(list);

	//printf(" node count is %d\n",count );
	while(temp->next!=NULL){
		
		//gia sigekrimeno io metraw se diastima [date1,date2]
		temp= temp->next;
		for (int i = 0; i < count; i++)
		{			
			if(strcmp(virus,temp->entryName) == 0){	

				num_of_nodes = find_count_in_range(temp->root->root,parameter1,parameter2);

			} 

		}			
		
	}
	// printf("%s %d\n",virus,num);
	return num_of_nodes;

}


int disease_frequency_with_param(LinkedList* diseaselist,char *virus ,char *parameter1 , char* parameter2 , char* vcountry){

	int count = 0;
	int num = 0;
	int num_of_nodes = 0;
	int sum = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = diseaselist;
	count = get_node_count(diseaselist);

	for (int i = 0; i < count ; i++){	
	// printf(" node count is %d\n",count );
		while(temp->next!=NULL){
			
		temp= temp->next;
				
			if(strcmp(virus,temp->entryName) == 0){


					//prepei na phgainw se olous tous komvous toy dentrou
					num_of_nodes = get_tree_nodes_count(temp->root->root);

					for(int k=0; k < num_of_nodes ; k++){


						sum = find_count_in_range_for_country(temp->root->root,parameter1,parameter2,vcountry);


					} 	
					
			} 
		}			
			
	}

	//printf("%s has at %s: %d cases \n",virus,vcountry,sum);
	//printf("%s %d\n",virus,sum);
	return sum;

 }

int searchPatientRecord(LinkedList *list , char* ID){

	int num = 0;
	LinkedList * temp = list;

	int count = get_node_count(list);

	while(temp!=NULL){


		temp= temp->next;


		if(strcmp(temp->root->root->data->recordID,ID) == 0){

			//printf("%s\n",ID );
			printf("%s %s %s %s %s \n",temp->root->root->data->recordID,temp->root->root->data->patientFirstName,temp->root->root->data->patientLastName,temp->root->root->data->diseaseID,temp->root->root->data->age);		
			fflush(stdout);

			break;

		}
		else {
			num++;
	    	printf("NOT FOUND in worker with pid %ld\n",(long)temp->pid);
			break;
		}
		
	}

	return num;
}



int numPatientAdmissions(LinkedList* list,char *virus ,char *parameter1 , char* parameter2){


	int count = 0;
	int num = 0;
	int num_of_nodes = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	count = get_node_count(list);

	// printf(" node count is %d\n",count );
	while(temp->next!=NULL){
		
		//gia sigekrimeno io metraw posa nodes exei to dentro se diastima [date1,date2]
		temp= temp->next;
		for (int i = 0; i < count ; i++)
		{			
			//printf("%s in %s \n",temp->entryName,temp->data->countryname);
			if(strcmp(virus,temp->entryName) == 0){	

				//prepei na phgainw se olous tous komvous toy dentrou
				num_of_nodes = get_tree_nodes_count(temp->root->root);
				//printf("nodes %d\n",num_of_nodes );

				for(int k=0; k < num_of_nodes ; k++){

					num = find_count_in_range_for_stats(temp->root->root,parameter1,parameter2);
					
					
				} 	


			} 

		}			
		
	}

	return num;
	//printf("%s NUM %d\n",temp->data->countryname,num);


}

int numPatientAdmissionsParam(LinkedList* list,char *virus ,char *parameter1 , char* parameter2 , char* vcountry){



	int count = 0;
	int num = 0;
	int num_of_nodes = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	count = get_node_count(list);

	while(temp->next!=NULL){
		
		//gia sigekrimeno io metraw posa nodes exei to dentro se diastima [date1,date2]
		temp= temp->next;
		for (int i = 0; i < count; i++)
		{	

			if(strcmp(virus,temp->entryName) == 0){	

				//prepei na phgainw se olous tous komvous toy dentrou
				num_of_nodes = get_tree_nodes_count(temp->root->root);

				for(int k=0; k < num_of_nodes ; k++){

					if((strcmp(temp->root->root->data->countryname,vcountry) == 0))
					num = find_count_in_range_for_stats(temp->root->root,parameter1,parameter2);


				} 	


			}
		}				
		
	}
	//printf("%s NUM %d\n",temp->data->countryname,num);

	return num;
	

}

int numPatientDischarges(LinkedList* list,char *virus ,char *parameter1 , char* parameter2){


	int count = 0;
	int num = 0;
	int num_of_nodes = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	count = get_node_count(list);

	// printf(" node count is %d\n",count );
	while(temp->next!=NULL){
		
		//gia sigekrimeno io metraw posa nodes exei to dentro se diastima [date1,date2]
		temp= temp->next;
		for (int i = 0; i < count; i++)
		{			
			if(strcmp(virus,temp->entryName) == 0){	

				//prepei na phgainw se olous tous komvous toy dentrou
				num_of_nodes = get_tree_nodes_count(temp->root->root);
				//printf("nides %d\n",num_of_nodes );

				for(int k=0; k < num_of_nodes ; k++){

					num = find_count_in_range_for_stats_exit(temp->root->root,parameter1,parameter2);


				} 	


			}

		}			
		
	}
	return num;
	//printf("%s %d\n",temp->data->countryname,num);

}

int numPatientDischargesParam(LinkedList* list,char *virus ,char *parameter1 , char* parameter2 , char* vcountry){

	int count = 0;
	int num = 0;
	int num_of_nodes = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	count = get_node_count(list);

	// printf(" node count is %d\n",count );
	while(temp->next!=NULL){
		
		//gia sigekrimeno io metraw posa nodes exei to dentro se diastima [date1,date2]
		temp= temp->next;
		for (int i = 0; i < count; i++)
		{			
			if(strcmp(virus,temp->entryName) == 0){	

				//prepei na phgainw se olous tous komvous toy dentrou
				num_of_nodes = get_tree_nodes_count(temp->root->root);
				//printf("nides %d\n",num_of_nodes );

				for(int k=0; k < num_of_nodes ; k++){

					if((strcmp(temp->root->root->data->countryname,vcountry) == 0))
					num = find_count_in_range_for_stats_exit(temp->root->root,parameter1,parameter2);


				} 	



			}

		}			
		
	}
	return num;

	//printf("%s %d\n",temp->data->countryname,num);

}

// // ------------------------------------------------voithitikes sinartiseis ---------------------------------------------------------	

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

int find_count_in_range(avltreenode *root, char* d1, char* d2) 
{ 
    if (root == NULL) return 0; 
  
    // if node is in range include it in count and recur for left and right children of it 
    if (to_seconds(root->data->filename) <= to_seconds(d2) && to_seconds(root->data->filename) >= to_seconds(d1)) 
        return 1 + find_count_in_range(root->left, d1, d2) + find_count_in_range(root->right, d1, d2); 
  
    // if current node is smaller than d1, then recur for right child 
    else if (to_seconds(root->data->filename) < to_seconds(d1)) 
        return find_count_in_range(root->right, d1, d2); 
  
    // else recur for left child 
    else return find_count_in_range(root->left, d1, d2); 
} 


int find_count_in_range_for_country(avltreenode *node, char *date1, char *date2, char *vcountry) {


    // an vrw to country pou thelw kai an d1<= entryDate <= d2
    int toReturn = 0;
    if((strcmp(node->data->countryname,vcountry) == 0 && 

		((to_seconds(node->data->filename) >= to_seconds(date1)) &&

		(to_seconds(node->data->filename) <= to_seconds(date2))) )){


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

int find_count_in_range_for_stats(avltreenode *node, char *date1, char *date2) {


    // an vrw to country pou thelw kai an d1<= entryDate <= d2
    int toReturn = 0;

    if((((to_seconds(node->data->filename) >= to_seconds(date1)) &&

		(to_seconds(node->data->filename) <= to_seconds(date2))) && strcmp(node->data->patientStatus,"ENTER") == 0 )){

        toReturn = 1;
    } 

    if (node->right == NULL && node->left == NULL)
        return toReturn;


	else if (node->left == NULL)

        return toReturn + find_count_in_range_for_stats(node->right, date1, date2);

    else if (node->right == NULL)

        return toReturn + find_count_in_range_for_stats(node->left, date1, date2); 
    
    else
        return toReturn + find_count_in_range_for_stats(node->right, date1, date2) +

               find_count_in_range_for_stats(node->left, date1, date2);


}

int find_count_in_range_for_stats_exit(avltreenode *node, char *date1, char *date2) {


    // an vrw to country pou thelw kai an d1<= entryDate <= d2
    int toReturn = 0;

    if((strcmp(node->data->patientStatus,"EXIT") == 0 && 

		((to_seconds(node->data->filename) >= to_seconds(date1)) &&

		(to_seconds(node->data->filename) <= to_seconds(date2))) )){

        toReturn = 1;
    } 

    if (node->right == NULL && node->left == NULL)
        return toReturn;


	else if (node->left == NULL)

        return toReturn + find_count_in_range_for_stats(node->right, date1, date2);

    else if (node->right == NULL)

        return toReturn + find_count_in_range_for_stats(node->left, date1, date2); 
    
    else
        return toReturn + find_count_in_range_for_stats(node->right, date1, date2) +

               find_count_in_range_for_stats(node->left, date1, date2);


}


