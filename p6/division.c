////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c, mySigHandler.c, sendsig.c
// This File:        division.c
// Other Files:      sendsig.c , mySigHandler.c
//
// Semester:         CS 354 Spring 2023
// Instructor:       Deppeler
// Author:           Navya Mongia
// GG:				 9
// Email:            mongia@wisc.edu
// CS Login:         navya
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NONE
// Online sources:   NONE
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for Spring 2023
//
//////////////////////////////////////////////////////////////////////////////// 
//necessary imports
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//necessary global variable for keeping track of total successful operations
int success_num = 0;

/**
 * This method detects a termination of ctrcl-c from keyboard and then displays the number of sucessful operations before termination.
 */

 void handler_SIGINT()
 {
     printf("\n");
     printf("Total number of operations completed successfully: %i\n", success_num);
     printf("The program will be terminated.\n");
     exit(0);
 }

/**
 * This method detects a divide by zero operation, handles the error, reports number of successful operations and terminates smoothly.
 */

void handler_SIGFPE() 
{
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %i\n", success_num);
    printf("The program will be terminated.\n");
    exit(0);
}


int main(int argc, char *argv[])
{
	//variables to store result, remainder etc
    char str1[100]; //strings created for user input
    char str2[100];
    int first,second,remainder,result;
     second = 0;
     remainder = 0;
     result = 0;
	 first = 0;
	
	//initliasing sigaction sig_int and sog_fpe
    struct sigaction sig_fpe;
    memset(&sig_fpe, 0, sizeof(sig_fpe));
    sig_fpe.sa_handler = handler_SIGFPE;

    if(sigaction(SIGFPE, &sig_fpe, NULL) != 0) 
    {
        printf("Failure in binding of SIGFPE handler.\n");
        exit(1);
    }

    struct sigaction sig_int;
    memset(&sig_int, 0, sizeof(sig_int));
    sig_int.sa_handler = handler_SIGINT;

    if(sigaction(SIGINT, &sig_int, NULL) != 0) 
    {
            printf("Failure binding of SIGINT handler.\n");
            exit(1);
    }

    while(1) //infinite loop 
    {
        printf("Enter first integer: "); //getting denominator as input

        if(fgets(str1, 100, stdin) != NULL)
        {
            first = atoi(str1);
        }
        
        else 
        {
                printf("fgets() has failed.");
                exit(0);
        }

        printf("Enter second integer: "); //getting numerator as input

        if(fgets(str2, 100, stdin) != NULL)
        {
            second = atoi(str2);
        }
        
        else 
        {
                printf("fgets() has failed.");
            exit(0);
        }

        
        result = first / second;
        remainder = first % second;
        
		printf("%i / %i is %i with a remainder of %i\n", first, second, result, remainder); //displaying output
        
        success_num++; //incrementing counter
    }

    return 0;
}
