#include "comands.h"
#include "hashtable.h"
#include "list.h"

void commands(){

    char* command;
    char* parameter1;
    char* parameter2;
    char* virus;
    char* vcountry;
    char* ID;
    char* FirstName;
    char* LastName;


    char* line = NULL;
    size_t len = 0;

    putchar(' ');
    putchar('~');

    //read line and execute the given command
    while(getline(&line, & len, stdin) != EOF)
    {
         line = strtok(line, "\n");
         command = strtok(line, " ");

         if(command == NULL)
            {
                continue;
            }

         

        // /listCountries
        else if(strcmp(command, "/listCountries") == 0)  
        {
           

            listCountries();

        }

        // /diseaseFrequency virusName date1 date2 [country]
        else if(strcmp(command, "/diseaseFrequency") == 0) //
        {

            virus = strtok(NULL," ");
            parameter1 = strtok(NULL," "); 
            parameter2 = strtok(NULL," ");
            vcountry = strtok(NULL," ");


            if (vcountry == NULL)
            {
             disease_frequency();

            } else {
   
             disease_frequency_with_param();
            }

        }

        // /searchPatientRecord recordID
        else if(strcmp(command, "/searchPatientRecord") == 0) 
        {
            ID = strtok(NULL," ");

            searchPatientRecord();
        }

        // /numPatientAdmissions disease date1 date2 [country]
        else if(strcmp(command, "/numPatientAdmissions") == 0) 
        {
           
            virus = strtok(NULL," ");
            parameter1 = strtok(NULL," "); 
            parameter2 = strtok(NULL," ");
            vcountry = strtok(NULL," ");


            if (vcountry == NULL)
            {
             
             numPatientAdmissions();

            } else {
   
                numPatientAdmissionsParam();
            }

        }
        // /numPatientDischarges disease date1 date2 [country]
        else if(strcmp(command, "/numPatientDischarges") == 0) 
        {
            virus = strtok(NULL," ");
            parameter1 = strtok(NULL," "); 
            parameter2 = strtok(NULL," ");
            vcountry = strtok(NULL," ");


            if (vcountry == NULL)
            {
             
             numPatientDischarges();

            } else {

            numPatientDischargesParam();
             
            }

        }   
        // /exit
        else if(strcmp(command, "/exit") == 0)
        {
            printf("exiting\n");
            break;
        }
        else
        {
            //printf("Command doesn't exist. Please try another commad. \n");
            printf("error\n");
        }

        putchar(' ');
        putchar('~');
    }
    free(line);
}   