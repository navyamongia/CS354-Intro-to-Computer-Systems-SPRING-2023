///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
// add your own File Header information here (as provided in p2A or Commenting guide
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      NONE
// Semester:         CS 354 Spring 2023
// Instructor:       Debra Deppler
//
// Author:           Navya Mongia
// Email:            mongia@wisc.edu
// CS Login:         navya
// GG#:              9
//                   
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

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;   
  
/*
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {

int size;//declaring variable size
printf("Enter magic square's size (odd integer >=3)\n");
scanf("%d" , &size); //for user input of size
if(size % 2 == 0){//checking if size is odd or not
printf("Magic square size must be odd.\n");
exit(1);
}
else if( size < 3){//checking if size is >= 3
printf("Magic square size must be >= 3.\n");
exit(1);
}
return size; //returning the size
}
   
 /* 
 * Makes a magic square of size n using the 
 * Siamese magic square algorithm or alternate from assignment 
 *
 * n is the number of rows and columns
 *
 * returns a pointer to the completed MagicSquare struct.
 */
MagicSquare *generateMagicSquare(int n) {
int p , q,k,j,i,x,y;
int middle_size = n / 2; //calculating the middle size for our square
MagicSquare *m2; //ptr to the struct
m2 = malloc(sizeof(MagicSquare)); //allocating heap mem for the struct ptr
m2->size = n; //assigning the size 
int **magic_square ; 
magic_square = malloc(n * sizeof(int*)); 
//checking return value of malloc
 if(magic_square==NULL){ //checking if magic square is null
 printf("invalid square\n");
 exit(1);
 }

//allocating mem on heap to make it a 2D array
 for( i =0; i< n; i++)
     {
         *(magic_square + i) = malloc(n * sizeof(int));
     }  
	
 m2 -> magic_square = magic_square;

//assigning zero to all elements of the magic square

for( x = 0; x < n;x++){
	for( y = 0; y <n; y++){	 
*(*(magic_square+x)+y) = 0;
}
}

k = middle_size; //assigning the middle size to our column index
j=0; //row index is zero initially

for(i = 1 ; i <= n * n ; i++){
 *(*(magic_square + j) + k) = i; //initially assigning 1 to middle element of 0th row //siamese method
p = j--; //decreasing the initial row index and assigning it to a new row index variable
q = k++; //increasing the initial column index and assigning it to a new colmn index variable
if(j<0){ //checking if row index <0 
j = n-1;}

if(k>n-1){ //checking if column index > size -1
k =0;}

if( *(*(magic_square + j) + k) != 0){ //if the element in the square is already present at that specific location
k = q; //then row index is now q 
j = p+1; //column index is now p + 1 
} 
}


return m2; //returning ptr to the struct
   
} 

/* 
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	FILE *fptr = fopen(filename, "w"); //opening the file
	if(fptr == NULL) { //checking the return value of fopen to see if file opened or not
                printf("error while opening the file.\n");
                exit(1);
        }
	// writing to the file
	fprintf(fptr, "%d\n", magic_square->size);//writing the size first
	for(int n = 0; n< magic_square->size; n++) { //writing the completed magic square to the text file
		for(int m = 0; m < magic_square->size - 1; m++) {
			fprintf(fptr, "%d,", *(*(magic_square->magic_square + n) + m));
		}
	fprintf(fptr,"%d\n", *(*(magic_square->magic_square + n) + magic_square->size -1));
	}
	// closing the file
int close_var=fclose(fptr); //checking return value of fclose
	if(close_var != 0) { //checking if file closed properly
		printf("error while closing the file.\n");
		exit(1);
	}

} 


/* 
 * Generates a magic square of the user specified size and
 * outputs the square to the output filename.
 * 
 * Add description of required CLAs here
 */
int main(int argc, char **argv) {
    //  Check input arguments to get output filename
    if(argc != 2)
    {
    	printf("Usage: %s <output_filename>\n", *(argv));
		exit(1);
	}

    //  Get magic square's size from user
    int sizeofboard = getSize();
// Generate the magic square by correctly interpreting   
MagicSquare *m1 = generateMagicSquare(sizeofboard);
//writing to output file
fileOutputMagicSquare(m1, *(argv+1)) ;
    //freeing heap memory
    for(int x = 0; x < m1->size ; x++){
		  free(*(m1->magic_square + x));
}
free(m1->magic_square);
free(m1);
m1 = NULL;

return 0;
} 

// S23

