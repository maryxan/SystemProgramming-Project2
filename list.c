#include "list.h"


//------------------------funvtions to init and insert to the list ---------------------------------------------------
 LinkedList* allocate_list() {

    // Allocate memory for a Linkedlist pointer
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->next = NULL;
    return list;
}

void linkedlist_insert(LinkedList* list, entry* item , int isCountry) {

    //if list is null
    if (list->next == NULL) {

        LinkedList* node = allocate_list();
        avltree* root = create_tree(item);
        node->root = root;
        node->next = NULL;
        list->next = node;
        if(isCountry){
        strcpy(node->entryName, item->country);
        }else strcpy(node->entryName, item->diseaseID);

        return;
    }

    //insert given entry onto the linked list
    LinkedList* temp = list;
    while (temp->next!=NULL) {

        temp = temp->next;

        if(isCountry){

            if(strcmp(temp->entryName,item->country) == 0){

             temp->root->root = insert_to_tree(temp->root->root,item);
             return;

            }       
        }else {


            if(strcmp(temp->entryName,item->diseaseID) == 0){

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
        strcpy(node->entryName, item->country);
        }else strcpy(node->entryName, item->diseaseID);

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
