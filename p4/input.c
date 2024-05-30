/**
@file input
@author Ethan Browne, efbrowne
This file contains the readline method
*/

#include "input.h"

/**
Reads a single line of input from the given input file or console
@param fp the input file
@return a string inside a block of dynamically allocated memory
*/
char *readLine( FILE *fp )
{
    int ch;
    
    if (fp == NULL){
        ch = getchar();
    }
    
    //Check to see if there is any input to process
    if (ch == EOF) {
        return NULL;
    }
    //The list with starting capacity of 5
    int capacity = INITIAL_CAPACITY;
    //The starting length of the array
    int len = 0;
    //The list
    char *list = (char *)malloc(capacity * sizeof(char));
    while ( ch != '\n' && ch != EOF) {
        //If the list reaches capacity resize and double the capacity
        if ( len >= capacity ) {
            capacity *= INCREASE_FACTOR;
            char *newList = (char *)malloc(capacity * sizeof(char));
            memcpy( newList, list, len * sizeof(char));
            free(list);
            list = newList;
        }
        list[len++] = ch;
        if (fp == NULL){
            ch = getchar();
        }
        if (ch == '\n' || ch == EOF){
            if ( len >= capacity ) {
                capacity *= INCREASE_FACTOR;
                char *newList = (char *)malloc(capacity * sizeof(char));
                memcpy( newList, list, len * sizeof(char));
                free(list);
                list = newList;
            }
            list[len++] = '\0';
        }
    }
    return list;
}