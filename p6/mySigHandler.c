////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c, sendsig.c, division.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c
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
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>

int counter = 0;
int time_of_alarm = 4;
/**
 * handler for alarms, calls alarm at end causing infinite loop
 */

void handler_SIGALRM() 
{
    time_t current;
        time(&current);
	
	//printing out pid and current time 
    if(current != -1 && ctime(&current) != NULL)
    {
        printf("PID: %i CURRENT TIME: %s", getpid(), ctime(&current));
    }
	
	//checking if return value of time is correct or not
    else
    {
        printf("PID: %i CURRENT TIME: error", getpid());
    }
    alarm(time_of_alarm); //calling alarm again
}

/**
 *handler for ctrl-c signals
 */
void handler_SIGINT() 
{
    printf("\n");
    printf("SIGINT handled.\n");
    printf("SIGUSR1 was handled %i times. Exiting now.\n", counter);
    exit(0);
}

/**
 * handler for SIGUSR1, it increments the counter and prints a message
 */
void handler_SIGUSR1()
{
      printf("SIGUSR1 handled and counted!\n");
      counter++;
}
int main()
{
    struct sigaction sa; //creating a sigaction structure
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler_SIGALRM; //assigning the handler to alarm struct
	
	//checking return value of sigaction
    if(sigaction(SIGALRM, &sa, NULL) != 0)
    {
        printf("Failure in binding SIGALRM handler.\n");
        exit(1);
    }

    struct sigaction suser;
    memset(&suser, 0, sizeof(suser));
    suser.sa_handler = handler_SIGUSR1; //assigning SIGUSR struct

	//checking return value of sigaction
    if(sigaction(SIGUSR1, &suser, NULL) != 0)
    {
        printf("Failure in binding SIGUSR1 handler.\n");
        exit(1);
    }

    struct sigaction sig_int;
    memset(&sig_int, 0, sizeof(sig_int));
    sig_int.sa_handler = handler_SIGINT; //assigning SIGINT struct

	//checking return value of sigaction
    if(sigaction(SIGINT, &sig_int, NULL) != 0)
    {
            printf("Failure in binding SIGINT handler.\n");
            exit(1);
    }

    printf("PID and time print every 4 seconds.\n");
    printf("Type Ctrl-C to end the program.\n");

    alarm(time_of_alarm); //alarm at every 4 seconds in our case

    while(1); //infinite loop until ctrl - c signalled 

    return 0;
}
