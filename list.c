#include "list.h"
#include "commandsfunctions.h"

//------------------------funvtions to init and insert to the list ---------------------------------------------------
 LinkedList* allocate_list() {

    // Allocate memory for a Linkedlist pointer
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->next = NULL;
    return list;
}

void linkedlist_insert(LinkedList* list, entry* item, int isCountry) {

    //if list is null
    if (list->next == NULL) {

        LinkedList* node = allocate_list();
        avltree* root = create_tree(item);
        node->root = root;
        node->next = NULL;
        list->next = node;
        if(isCountry){
        strcpy(node->entryName, item->countryname);
        }else strcpy(node->entryName, item->diseaseID);
   		node->pid = item->workerpid;
        node->data = item;

        return;
    }

    //insert given entry onto the linked list
    LinkedList* temp = list;
    while (temp->next!=NULL) {

        temp = temp->next;
        
        if(isCountry){

            if(strcmp(temp->entryName,item->countryname) == 0){

             temp->data = item;   
             temp->root->root = insert_to_tree(temp->root->root,item);
             return;

            }       
        }else {


            if(strcmp(temp->entryName,item->diseaseID) == 0){

            temp->data = item;
             temp->root->root = insert_to_tree(temp->root->root,item);
             return;

            }      
        }

    }
    
    //if disease or country value doesnt exist create a new node
    LinkedList* node = allocate_list();
    avltree* root = create_tree(item);
    node->root = root;    
    node->next = NULL;
    temp->next = node;
    if(isCountry){
        strcpy(node->entryName, item->countryname);
        }else strcpy(node->entryName, item->diseaseID);
    node->pid = item->workerpid;
    node->data = item;

        

    return ;
}

// -----------------------------------------function to free the list--------------------------------------------
void free_linkedlist(LinkedList* list) {

    LinkedList* temp = list->next;
    LinkedList* temp1 = list;

    list = list->next;
    while (list!= NULL) {
        temp = list;
        list = list->next;
        avl_tree_free(temp->root->root);
        free(temp->root);
        free(temp);
    }
    free(temp1);
}


void print(LinkedList* list) {

	int count = 0;
	int num_of_nodes = 0;

	//arxizw apo to head tis listas	
	LinkedList * temp = list;
	count = get_node_count(list);

	while(temp->next!=NULL){
		
		//gia kathe komvo sti lista pou einai enas ios metraw posa nodes exei to dentro
		temp= temp->next;
		
		printf("%s %s %ld\n",temp->entryName,temp->data->countryname ,(long)temp->pid);
		
	}
   	
}


