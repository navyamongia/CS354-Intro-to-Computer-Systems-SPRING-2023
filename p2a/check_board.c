///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
// 09/20/2021 Revised to free memory allocated in get_board_size function.
// 01/24/2022 Revised to use pointers for CLAs
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        check_board.c
// This File:        check_board.c
// Other Files:      NONE
// Semester:         CS 354 Spring 2023
// Instructor:       Debra Deppler
//
// Author:           Navya Mongia
// Email:            mongia@wisc.edu
// CS Login:         navya
// GG#:              9
//                   (See https://canvas.wisc.edu/groups for your GG number)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   including family, friencs, classmates, tutors,
//                   Peer Mentors, TAs, and Instructor.
//
// Persons:          NONE
//
// Online sources:   NONE
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *DELIM = ",";  // commas ',' are a common delimiter character for data strings

/* COMPLETED (DO NOT EDIT):       
 * Read the first line of input file to get the size of that board.
 * 
 * PRE-CONDITION #1: file exists
 * PRE-CONDITION #2: first line of file contains valid non-zero integer value
 *
 * fptr: file pointer for the board's input file
 * size: a pointer to an int to store the size
 *
 * POST-CONDITION: the integer whos address is passed in as size (int *) 
 * will now have the size (number of rows and cols) of the board being checked.
 */
void get_board_size(FILE *fptr, int *size) {      
	char *line1 = NULL;
	size_t len = 0;

	if ( getline(&line1, &len, fptr) == -1 ) {
		printf("Error reading the input file.\n");
		free(line1);
		exit(1);
	}

	char *size_chars = NULL;
	size_chars = strtok(line1, DELIM);
	*size = atoi(size_chars);

	// free memory allocated for reading first link of file
	free(line1);
	line1 = NULL;
}



/* 
 * Returns 1 if and only if the board is in a valid Sudoku board state.
 * Otherwise returns 0.
 * 
 * A valid row or column contains only blanks or the digits 1-size, 
 * with no duplicate digits, where size is the value 1 to 9.
 * 
 * Note: p2A requires only that each row and each column are valid.
 * 
 * board: heap allocated 2D array of integers 
 * size:  number of rows and columns in the board
 */
int valid_board(int **board, int size) {


//outer for loop to iterate through the 2d board
for(int x = 0; x < size; x ++){
	for(int y = 0; y < size; y++){
		if(*(*(board+x) + y) != 0) //checking if the contents of the board is 0 or not
			{
			int size_to_check = size - y; //leftover size to check
			int size_to_check_col = size - x; //leftover column size to check
			int number_at_index = *(*(board+x) + y) ; //the content to check
			for(int m = 1; m< size_to_check; m++) //another loop to check duplicacy
			{ if( number_at_index == *(*(board+x) + y +m )) //if duplicacy found
				{

				
					return 0; //then returning 0 and exiting 1
					exit(1);
				} 
			}
			//another loop to check duplicacy in columns
			for(int n = 1; n < size_to_check_col ; n++)
			{
				//if duplicacy found
				if(number_at_index == *(*(board+x+n) + y))
				{
					
						return 0;
						exit(1);
				}			
			}
				
}
}

}
	return 1;//returning 1 to show that the board is valid}    

}

/* 
 * This program prints "valid" (without quotes) if the input file contains
 * a valid state of a Sudoku puzzle board wrt to rows and columns only.
 *
 * A single CLA is required, which is the name of the file 
 * that contains board data is required.
 *
 * argc: the number of command line args (CLAs)
 * argv: the CLA strings, includes the program name
 *
 * Returns 0 if able to correctly output valid or invalid.
 * Only exit with a non-zero result if unable to open and read the file given.
 */
int main( int argc, char **argv ) {              

	// Check if number of command-line arguments is correct.
	if(argc != 2) { 
	printf("invalid number of CLAs\n");

}  
	// Open the file and check if it opened successfully.
	FILE *fp = fopen(*(argv + 1), "r");
	if (fp == NULL) {
		printf("Can't open file for reading.\n");
		exit(1);
	}

	// declaring local variable size
	int size;

	// Call get_board_size to read first line of file as the board size.
	get_board_size(fp, &size);
if(size>9 || size<1){ //checking for size
  printf("invalid\n"); //if size invalid then exiting 1
  exit(1);
  }
	// Dynamically allocate a 2D array for given board size.
  int **board;
  board = malloc(size * sizeof(int*)); //allocating on heap
 if(board==NULL){ //checking if board is null
 printf("invalid\n");
 exit(1);
 }
//alloacting the rows on heap
 for(int i =0; i< size; i++)
     {
         *(board + i) = malloc(size * sizeof(int));
     }	
if(board == NULL) 
{
 printf("invalid\n");
exit(1);
}	    
	// Read the rest of the file line by line.
	// Tokenize each line wrt the delimiter character 
	// and store the values in your 2D array.
	char *line = NULL;
	size_t len = 0;
	char *token = NULL;
	for (int i = 0; i < size; i++) {

		if (getline(&line, &len, fp) == -1) {
			printf("Error while reading line %i of the file.\n", i+2);
			exit(1);
		}

		token = strtok(line, DELIM);
		for (int j = 0; j <  size; j++) {
			// Complete the line of code below
			// to initialize your 2D array.
				*(*(board+i) + j)= atoi(token);
			token = strtok(NULL, DELIM);
		}
	}

	// Calling the function valid_board and print the appropriate
	//output depending on the function's return value.
	int result = valid_board(board,  size);
	if(result == 0){ //if 0 then invalid
		printf("invalid\n");
	}
	else{ //1 if board is valid
		printf("valid\n");
	}

	// Freeing all dynamically allocated memory.
    for(int i =0; i<size; i++){
    free(*(board+i));
 }
free(board); //freeing the board
board = NULL;
free(line); //freeing line to prevent mem leaks
line = NULL;
	//Closing the file.
	if (fclose(fp) != 0) {
		printf("Error while closing the file.\n");
		exit(1);
	} 

	return 0;       
}       

