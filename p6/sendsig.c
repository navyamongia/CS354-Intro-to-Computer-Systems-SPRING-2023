////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c, mySigHandler.c, division.c
// This File:        sendsig.c
// Other Files:      division.c, mySigHandler.c
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
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
    if(argc != 3) 
    {
        printf("Usage: sendsig <signal type> <pid>\n");
        exit(1);
    }

    if(strcmp(argv[1], "-i") == 0)
    {
        if(kill(atoi(argv[2]), SIGINT) == -1)
        {
            printf("Error.\n");
            exit(0);
        }
    }

    else if(strcmp(argv[1], "-u") == 0)
    {
        if(kill(atoi(argv[2]), SIGUSR1) == -1)
        {
            printf("Error.\n");
            exit(0);
        }
    }

    else
    {
        printf("Usage: sendsig <signal type> <pid>\n");
        exit(0);
    }

    return 0;
}
