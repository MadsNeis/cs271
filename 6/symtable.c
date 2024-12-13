/****************************************
 * Unit 6 Project for CS 271
 *
 * [NAME] Madison Neiswonger
 * [TERM] FALL 2024
 *
 ****************************************/
#include "hack.h"
#include "symtable.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Hash Function
struct Symbol* hashArray[SYMBOL_TABLE_SIZE] = { NULL };
 int hash(const char* str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return (int)(hash % SYMBOL_TABLE_SIZE);
}


 //Insert Function 
 void symtable_insert(char* key, hack_addr addr) {
     struct Symbol *item = (struct Symbol*)malloc(sizeof(struct Symbol));
     //doesn't like address, switched to addr
     item->addr = addr;
     item->name = strdup(key);

     int hashIndex = hash(key);

     while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->name) {

         ++hashIndex;

         hashIndex %= SYMBOL_TABLE_SIZE;
     }
     hashArray[hashIndex] = item;

 }
     

 //Find Function:
 Symbol* symtable_find(char* key) {
     int index = hash(key);

     while (hashArray[index] != NULL) {
         if (strcmp(hashArray[index]->name, key) == 0) {
             return hashArray[index];
         }
         //index = (index + 1) % SYMBOL_TABLE_SIZE;
         ++index;

         index %= SYMBOL_TABLE_SIZE;
     }
     return NULL;
 }

 //symtable_display_table function: 
 // Debugging. Removes call to it before submitting it to gradescorpe.
 /*void symtable_display_table() {
     for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
         if (hashArray[i] != NULL) {
             printf("{%s,%d}\n", hashArray[i]->name, hashArray[i]->addr);
         }
     }
 }*/

 //Print Labels Function:
 void symtable_print_labels() {
     int i = 0;
     for (i = 0; i < SYMBOL_TABLE_SIZE; i++) {
         if (hashArray[i] != NULL) {
             //printf("{%s,%d}\n", hashArray[i]->name, hashArray[i]->addr);
         }
     }
 }


