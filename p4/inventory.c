/**
@file Inventory
@author Ethan Browne, efbrowne
This file contains all of the functions that have to deal with the inventory structs including printing their contents and reading into them.
*/

#include "inventory.h"

//Number of inputs on the first line of the record file
#define LINE_ONE_ARG 3

/**
Dynamically allocates storage for the Inventory
Initializes its fields
@return a pointer to the new Inventory
*/
Inventory *makeInventory()
{
    Inventory *inv = malloc(sizeof(Inventory));
    inv->list = (Record *)malloc(INITIAL_CAPACITY * sizeof(Record));
    inv->count = 0;
    inv->capacity = INITIAL_CAPACITY;
    return inv;
}

/**
Frees the memory used to store the given Inventory
@param inventory the inventory to deallocate
*/
void freeInventory( Inventory *inventory )
{
    free(inventory->list);
    free(inventory);
}

/**
Reads all the records from a file
Can also update the number of copies of a record with the same id that is already stored in the inventory.
@param filename the file to read from
@param inventory makes an instance of the Record struct for a record in the file and stores a pointer to that Record in the resizable array in inventory
*/
void readRecords( char const *filename, Inventory *inventory )
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        //INVALID FILE
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(EXIT_FAILURE);
        //Don't need to close the file since it was never open
    }
    char * recordLine = readLine(fp);
    while (recordLine != NULL){
        bool inList = false;
        int id = 0;
        //CAN I INITIALIZE SO THAT ALL CHARACTERS CONTAIN NULL TERMINATOR = {} OR WILL I NOT BE ABLE TO SCAN IN
        char artist[ARTIST_TITLE_SIZE + 1] = {};
        char title[ARTIST_TITLE_SIZE + 1] = {};
        char genre[GENRE_SIZE + 1] = {};
        int copies = 0;

        if (sscanf(recordLine, "%d%s%d", &id, genre, &copies) != LINE_ONE_ARG) {
            //INVALID FILE
            fprintf(stderr, "Invalid record file: %s\n", filename);
            fclose(fp);
            freeInventory(inventory);
            exit(EXIT_FAILURE);
        }
        free(recordLine);
        recordLine = readLine(fp);
        if (recordLine == NULL){
            //INVALID FILE
            fprintf(stderr, "Invalid record file: %s\n", filename);
            fclose(fp);
            freeInventory(inventory);
            exit(EXIT_FAILURE);
        }
        strcpy(artist, recordLine);
        if (strlen(artist) == 0) {
            //INVALID FILE
            fprintf(stderr, "Invalid record file: %s\n", filename);
            fclose(fp);
            freeInventory(inventory);
            exit(EXIT_FAILURE);
        }
        free(recordLine);
        recordLine = readLine(fp);
        if (recordLine == NULL){
            //INVALID FILE
            fprintf(stderr, "Invalid record file: %s\n", filename);
            fclose(fp);
            freeInventory(inventory);
            exit(EXIT_FAILURE);
        }
        strcpy(title, recordLine);
        if (strlen(title) == 0) {
            //INVALID FILE
            fprintf(stderr, "Invalid record file: %s\n", filename);
            fclose(fp);
            freeInventory(inventory);
            exit(EXIT_FAILURE);
        }

        if (id <= 0 || artist[ARTIST_TITLE_SIZE] != '\0' || title[ARTIST_TITLE_SIZE] != '\0' || genre[GENRE_SIZE] != '\0' || copies < 0) {
            //INVALID FILE
            fprintf(stderr, "Invalid record file: %s\n", filename);
            fclose(fp);
            freeInventory(inventory);
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < (*inventory).count; i++){
            if ((*inventory).list[i].id == id) {
                if (strcmp((*inventory).list[i].artist, artist) != 0 || strcmp((*inventory).list[i].title, title) != 0 || strcmp((*inventory).list[i].genre, genre) != 0){
                    //INVALID FILE
                    fprintf(stderr, "Invalid record file: %s\n", filename);
                    fclose(fp);
                    freeInventory(inventory);
                    exit(EXIT_FAILURE);
                } else {
                    (*inventory).list[i].copies += copies; //THIS MIGHT BE WRONG
                    inList = true;
                }
            }
        }

        if (!inList){
            if ((*inventory).count >= (*inventory).capacity){
                (*inventory).capacity *= INCREASE_FACTOR;
                Record *newList = (Record *)malloc((*inventory).capacity * sizeof(Record));
                memcpy( newList, (*inventory).list, (*inventory).count * sizeof(Record));
                free((*inventory).list);
                (*inventory).list = newList;
            }
            Record rec = {id, {}, {}, {}, copies};
            strcpy(rec.artist, artist);
            strcpy(rec.title, title);
            strcpy(rec.genre, genre);

            (*inventory).list[(*inventory).count++] = rec;
        }
        free(recordLine);
        recordLine = readLine(fp);
    }
    fclose(fp);
}

/**
Uses the qsort() function together with the function pointer parameter to order the records.
@param inventory pointer to the inventory
@param compare pointer to the comparison function
*/
void sortRecords( Inventory *inventory, int (* compare) (void const *va, void const *vb ))
{
    qsort((*inventory).list, (*inventory).count, sizeof(Record), compare);
}

/**
Function prints all or some of the records. It uses the test() function pointer parameter together with the string, str, to decide which records to print
@param inventory pointer to the inventory
@param test pointer to the function
@param str pointer to the constant string that detrmines whether or not to print each record
*/
void listRecords( Inventory *inventory, bool (*test)( Record const *record, char const *str ), char const *str )
{
    printf("ID  Artist                         Title                          Genre        Copies\n");
    for (int i = 0; i < (*inventory).count; i++) {
        if (test(&((*inventory).list[i]), str)){
            printf("%3d", (*inventory).list[i].id);
            printf(" %-31s", (*inventory).list[i].artist);
            printf("%-31s", (*inventory).list[i].title);
            printf("%-13s", (*inventory).list[i].genre);
            printf("%6d\n", (*inventory).list[i].copies);
        }
    }
}