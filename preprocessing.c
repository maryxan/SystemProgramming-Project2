#include "preprocessing.h"
#include "namedpipes.h"
#include "commandsfunctions.h"
#include "avl.h"
#include "date.h"

Stats** preprocessing(char* inputfile,pid_t workerpid,LinkedList* countrylist,LinkedList* diseaselist,char* filename,disease_node *head){

	  char recordID[100];
    char patientStatus[100];
	  char patientFirstName[100];
    char patientLastName[100];
    char diseaseID[100];
    char age[100];

    char *temp;

    FILE *fp;
    char* line = NULL;
    size_t len = 0; 

    

    fp = fopen(inputfile,"r");

    if(fp == NULL){

        printf("Could not open file\n"); 

    }else {

        //when its open for every entry call preprocessing and feed structs

        int rep = 0;
        int idrep = 0;
        temp = strtok(inputfile + 7,"/"); // pairnw to onoma tis xwras

        while(getline(&line, &len, fp) != EOF) {


        entry* info = malloc (sizeof(entry)); // ftiaxnw ena struct me ta stoixeia tis kathe eggrafis
    
        sscanf(line,"%s %s %s %s %s %s",info->recordID, info->patientStatus, info->patientFirstName, info->patientLastName, info->diseaseID, info->age);
        

        strcpy(info->filename ,filename);
        info->workerpid = workerpid;
        strcpy(info->countryname,temp);

          
        if(strcmp(info->patientStatus,"ENTER") == 0){

            strcpy(info->entrydate,filename);
            strcpy(info->stat,"0"); 

        }else if(strcmp(info->patientStatus,"EXIT") == 0) {

            strcpy(info->exitdate,filename);    
            strcpy(info->stat,"1"); 

            if(check_record_id(countrylist,info->recordID,filename)){

              printf("ERROR\n");
              continue;

            }   


        }

        entry *ref1 , *ref2;

        ref1 = info;
        ref2 = info;
       
        linkedlist_insert(diseaselist,ref1,0);
        linkedlist_insert(countrylist,ref2,1);
       

        //feed statistics list
        disease_node *temp_dis = head->next;
        disease_node *temp_dis2 = head;
        while (temp_dis!=NULL){

          if (strcmp(temp_dis->stats->diseaseID,info->diseaseID)==0){

              if(atoi(info->age) >= 0 && atoi(info->age) <= 20){
                   temp_dis->stats->range1++;
                }
                else if(atoi(info->age) >= 21 && atoi(info->age) <= 40){
                    temp_dis->stats->range2++;
                }
                else if(atoi(info->age) >= 41 && atoi(info->age) <= 60){
                    temp_dis->stats->range3++;
                }
                else if(atoi(info->age) > 60){
                    temp_dis->stats->range4++;
                }
                break;
          }
          temp_dis2 = temp_dis;
          temp_dis=temp_dis->next;
        }
        if (temp_dis == NULL){
            temp_dis2->next = malloc(sizeof(disease_node));
            temp_dis2->next->stats = malloc(sizeof(Stats));
            strcpy(temp_dis2->next->stats->diseaseID,info->diseaseID);
            temp_dis2->next->next = NULL;
            temp_dis2->next->stats->range1=0;
            temp_dis2->next->stats->range2=0;
            temp_dis2->next->stats->range3=0;
            temp_dis2->next->stats->range4=0;
            if(atoi(info->age) >= 0 && atoi(info->age) <= 20){
               temp_dis2->next->stats->range1++;
            }
            else if(atoi(info->age) >= 21 && atoi(info->age) <= 40){
                temp_dis2->next->stats->range2++;
            }
            else if(atoi(info->age) >= 41 && atoi(info->age) <= 60){
                temp_dis2->next->stats->range3++;
            }
            else if(atoi(info->age) > 60){
                temp_dis2->next->stats->range4++;
            }
        }

        // free(temp_dis2->next->stats);
        // free(temp_dis2->next);

      } //end of while

    }

    fclose(fp);
    free(line);

}



// //checks if a given record ID already exists
int check_record_id(LinkedList *list, char* ID , char* filename){

  int count = 0;
  int num = 0;
  int num_of_nodes = 0;

  //arxizw apo to head tis listas 
  LinkedList * temp = list;
  count = get_node_count(list);


  while(temp->next!=NULL){
   
          temp= temp->next;

          for (int i = 0; i < count; i++){   

          
                  num_of_nodes = get_tree_nodes_count(temp->root->root);

                  for(int k=0; k < num_of_nodes ; k++){

                      //psaxnw gia to ID
                      if(strcmp(temp->root->root->data->recordID,ID) == 0){

                        if (strcmp(temp->root->root->data->patientStatus,"ENTER") == 0)
                        {

                         if(to_seconds(temp->root->root->data->filename) <= to_seconds(filename)){

                            return 0;

                         }else return 1;

                        }else{

                          return 1;
                        }

                      }
                      
                  }   

          }

          
          
    }

    return 1;

}