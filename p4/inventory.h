/**
@file Inventory
@author Ethan Browne, efbrowne
This is the header file for the inventory.c file
It contains the structs for inventorys, records, orders, and orderitems.
It also contains function prototypes and constants
*/

/** Size of Artist and TITLE name*/
#define ARTIST_TITLE_SIZE 30
/** Size of Genre Name */
#define GENRE_SIZE 12

#include "input.h"


//ORDER AND ORDER ITEM STRUCTS I DONT KNOW WHERE TO PUT THEM YET SO LEAVE THEM HEAR FOR NOW

/** Record struct. Contains an id, artist, title, genre, and number of copies */
typedef struct Record{
    int id;
    char artist[ARTIST_TITLE_SIZE + 1];
    char title[ARTIST_TITLE_SIZE + 1];
    char genre[GENRE_SIZE + 1];
    int copies;
} Record;

/** OrderItem struct. Contains a pointer to a record and the number of copies they are purchasing */
typedef struct OrderItem{
    Record *record;
    int copies;
} OrderItem;

/** Order struct. Contains a resizeable array that starts at size 5. */
typedef struct Order{
    OrderItem *list;
    int count;
    int capacity;
} Order;

/** Inventory struct. Contains a list of records, a capacity of the list, and the count of records in the list */
typedef struct Inventory{
    Record *list;
    int count; 
    int capacity;
} Inventory;


/**
Dynamically allocates storage for the Inventory
Initializes its fields
@return a pointer to the new Inventory
*/
Inventory *makeInventory();

/**
Frees the memory used to store the given Inventory
@param inventory the inventory to deallocate
*/
void freeInventory( Inventory *inventory );

/**
Reads all the records from a file
Can also update the number of copies of a record with the same id that is already stored in the inventory.
@param filename the file to read from
@param inventory makes an instance of the Record struct for a record in the file and stores a pointer to that Record in the resizable array in inventory
*/
void readRecords( char const *filename, Inventory *inventory );

/**
Uses the qsort() function together with the function pointer parameter to order the records.
@param inventory pointer to the inventory
@param compare pointer to the comparison function
*/
void sortRecords( Inventory *inventory, int (* compare) (void const *va, void const *vb ));

/**
Function prints all or some of the records. It uses the test() function pointer parameter together with the string, str, to decide which records to print
@param inventory pointer to the inventory
@param test pointer to the function
@param str pointer to the constant string that detrmines whether or not to print each record
*/
void listRecords( Inventory *inventory, bool (*test)( Record const *record, char const *str ), char const *str );