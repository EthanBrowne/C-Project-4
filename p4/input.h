/**
@file input
@author Ethan Browne, efbrowne
This is the header file for the input.c file
*/

//Constant that represents the capacity of the list in inventory, order, and the line
#define INITIAL_CAPACITY 5
//Constant that represents the factor to increase the capacity by
#define INCREASE_FACTOR 2

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


/**
Reads a single line of input from the given input file or console
@param fp the input file
@return a string inside a block of dynamically allocated memory
*/
char *readLine( FILE *fp );