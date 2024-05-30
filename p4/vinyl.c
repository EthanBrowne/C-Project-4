/**
@file vinyl
@author Ethan Browne, efbrowne
This file contains the main method and functions needed to sort and list records
*/

#include "input.h"
#include "inventory.h"

//Number of int arguements for add and remove methods
#define ADD_REMOVE_ARG 2

/**
Dynamically allocates storage for the Order
Initializes its fields
@return a pointer to the new Order
*/
Order *makeOrder()
{
    Order *ord = malloc(sizeof(Order));
    ord->list = (OrderItem *)malloc(INITIAL_CAPACITY * sizeof(OrderItem));
    ord->count = 0;
    ord->capacity = INITIAL_CAPACITY;
    return ord;
}

/**
Frees the memory used to store the given Order
@param order the order to deallocate
*/
void freeOrder( Order *order )
{
    for (int i = (*order).count - 1; i >= 0; i--) {
        free((*order).list[i].record);
    }
    free(order->list); //NEED TO FREE THE ORDERITEMS
    free(order);
}

/**
Compares two records id
@param va pointer to the first record
@param vb pointer to the second record
@return which record is greater than the other or whether they are equal
*/
int static compareID(void const *r1, void const *r2 )
{
    Record const * va = (Record const *) r1;
    Record const * vb = (Record const *) r2;
    if ((*va).id > (*vb).id) {
        return 1;
    } else if ((*va).id < (*vb).id) {
        return -1;
    }
    return 0;
}

/**
Compares two records artists, then title, then id
@param va pointer to the first record
@param vb pointer to the second record
@return which record is greater than the other or whether they are equal
*/
int static compareATI(void const *r1, void const *r2 )
{
    Record const * va = (Record const *) r1;
    Record const * vb = (Record const *) r2;
    if (strcmp((*va).artist, (*vb).artist) != 0){
        return strcmp((*va).artist, (*vb).artist);
    } else if (strcmp((*va).title, (*vb).title) != 0){
        return strcmp((*va).title, (*vb).title);
    } else if ((*va).id > (*vb).id) {
        return 1;
    } else if ((*va).id < (*vb).id) {
        return -1;
    }
    return 0;
}

/**
This function compares the genre of the record to that of the string
@param record pointer to record 
@param str pointer to genre
@return wether the genre of the record matches the string
*/
bool testListGenre(Record const *record, char const *str)
{
    return strcmp((*record).genre, str) == 0;
}

/**
Prints out a list of all of the records in the inventory with a specific genre. Sorted by artists, then title, then id
@param genre the genre to list
*/
void listGenre(char genre[GENRE_SIZE + 1], Inventory *inventory)
{
    sortRecords(inventory, compareATI);
    listRecords(inventory, testListGenre, genre);
}

/**
This function always returns true as all of the records in the list will be listed
@param record pointer to record 
@param str pointer to string
@return true
*/
bool testList(Record const *record, char const *str)
{
    return true;
}

/**
Lists all of the records in the inventory. They will be sorted by id
*/
void list(Inventory *inventory)
{
    sortRecords(inventory, compareID);
    listRecords(inventory, testList, "");
}

/**
Lists all of the records in the order. They will be sorted by when they were added to the order
*/
void listOrder(Order *order)
{
    printf("ID  Artist                         Title                          Genre        Copies\n");
    for (int i = 0; i < (*order).count; i++) {
        if ((*order).list[i].copies > 0){
            printf("%3d", (*(*order).list[i].record).id);
            printf(" %-31s", (*(*order).list[i].record).artist);
            printf("%-31s", (*(*order).list[i].record).title);
            printf("%-13s", (*(*order).list[i].record).genre);
            printf("%6d\n", (*order).list[i].copies);
        }
    }
}

/**
This function adds a record to the order
@param order the order to add to
@param id the id of the record to add
@param copies the number of copies to add
@return whether the addition was valid
*/
bool add(Order *order, Inventory *inventory, int id, int copies)
{
    if (copies <= 0) {
        return false;
    }
    for (int j = 0; j < (*inventory).count; j++) {
        if ((*inventory).list[j].id == id) {
            //If the record is already in the order increase the number of copies
            for (int i = 0; i < (*order).count; i++) {
                if ((*(*order).list[i].record).id == id) {
                    if (copies +  (*order).list[i].copies > (*inventory).list[j].copies) {
                        return false;
                    }
                    (*order).list[i].copies += copies;
                    return true;
                }
            }
            //If it is not already in the order find it in the inventory and add it to the order
            if (copies > (*inventory).list[j].copies) {

                return false;
            }
            Record *newRec = malloc(sizeof(Record));
            newRec->id = (*inventory).list[j].id;
            strcpy(newRec->artist, (*inventory).list[j].artist);
            strcpy(newRec->title, (*inventory).list[j].title);
            strcpy(newRec->genre, (*inventory).list[j].genre);
            newRec->copies = (*inventory).list[j].copies;

            OrderItem item = {newRec, copies};


            if ((*order).count >= (*order).capacity){
                (*order).capacity *= INCREASE_FACTOR;
                OrderItem *newList = (OrderItem *)malloc((*order).capacity * sizeof(OrderItem));
                memcpy( newList, (*order).list, (*order).count * sizeof(OrderItem));
                free((*order).list);
                (*order).list = newList;
            }
            (*order).list[(*order).count++] = item;
            return true;
        }
    }
    return false;
}

/**
This function removes a record from the order
@param order the order to remove from
@param id the id of the record to remove
@param copies the number of copies to remove
@return whether the removal was valid
*/
bool removeRecord(Order *order, int id, int copies)
{
    if (copies <= 0) {
        return false;
    }
    for (int i = 0; i < (*order).count; i++) {
        if ((*(*order).list[i].record).id == id) {
            if ((*order).list[i].copies >= copies){
                (*order).list[i].copies -= copies;
            } else {
                return false;
            }
            return true;
        }
    }
    return false;
}

/**
This function decreases the inventory by the number of copies of each record the user is buying
@param order the order to checkout with
*/
void checkOut(Order **order, Inventory *inventory)
{
    for (int i = 0; i < (*order)->count; i++) {
        for (int j = 0; j < inventory->count; j++) {
            if ((*order)->list[i].record->id == inventory->list[j].id) {
                inventory->list[j].copies -= (*order)->list[i].copies;
            }
        }
    }
    // Create new blank order
    freeOrder(*order);
    *order = makeOrder();
}

/**
The main method
@param argc the number of arguements
@param argv list of strings that are the arguements
@return whether the program was run successfully
*/
int main( int argc, char *argv[])
{
    //If there is no record files fail
    if (argc <= 1) {
        fprintf(stderr, "usage: vinyl record-file+\n");
        exit(EXIT_FAILURE);
    }

    //Creates an empty inventory and order
    Inventory *inventory = makeInventory() ;

    //Read all of the records from all of the files. If there is an error in files it will fail out within
    for (int i = 1; i < argc; i++){
        readRecords((argv[i]), inventory);
    }

    Order *order = makeOrder();

    char *line = readLine(NULL);
    printf("cmd> ");
    while (line != NULL) {
        char command[GENRE_SIZE + 1] = {};
        int num = 0;
        int num2 = 0;

        // Number of characters read
        int offset = 0;
        int n = 0;

        printf("%s\n", line);
        if (sscanf(line, "%s%n", command, &n) == 1){
            offset += n;
            if (strcmp(command, "list") == 0) {
                if (sscanf(line + offset, "%s%n", command, &n) == 1) {
                    if (strcmp(command, "genre") == 0) {
                        offset += n;
                        if (sscanf(line + offset, "%s", command) == 1) { //MIGHT NOT GET WHOLE GENRE AND MIGHT NOT NEED GENRE
                            listGenre(command, inventory);
                        } else {
                            printf("Invalid command\n");
                        }
                    } else if (strcmp(command, "order") == 0) {
                        listOrder(order);
                    } else {
                        printf("Invalid command\n");
                    }
                } else {
                    list(inventory);
                }
            } else if (strcmp(command, "add") == 0) {
                if (sscanf(line + offset, "%d%d", &num, &num2) == ADD_REMOVE_ARG) {
                    if (!add(order, inventory, num, num2)){
                        printf("Invalid command\n");
                    }
                } else {
                    printf("Invalid command\n");
                }
            } else if (strcmp(command, "remove") == 0) {
                if (sscanf(line + offset, "%d%d", &num, &num2) == ADD_REMOVE_ARG) {
                    if (!removeRecord(order, num, num2)){
                        printf("Invalid command\n");
                    }
                } else {
                    printf("Invalid command\n");
                }
            } else if (strcmp(command, "checkout") == 0) {
                checkOut(&order, inventory);
            } else if (strcmp(command, "quit") == 0) {
                free(line);
                freeOrder(order);
                freeInventory(inventory);
                return EXIT_SUCCESS;
            } else {
                printf("Invalid command\n");
            }
        } else {
            printf("Invalid command\n");
        }
        free(line);
        line = readLine(NULL);
        printf("\ncmd> ");
    }
    free(line);
    freeOrder(order);
    freeInventory(inventory);
    return EXIT_SUCCESS;
}