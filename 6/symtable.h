/****************************************
 * Unit 6 Project for CS 271
 *
 * [NAME] Madison Neiswonger
 * [TERM] FALL 2024
 *
 ****************************************/

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef int16_t hack_addr;
typedef int16_t opcode;

//Structure to represent symbol
typedef struct Symbol {
	char* name;
	hack_addr addr;
} Symbol;

#define SYMBOL_TABLE_SIZE 1000

extern Symbol* hashArray[SYMBOL_TABLE_SIZE];

int hash(const char* str);
Symbol* symtable_find(char* key);
void symtable_insert(char* key, hack_addr addr);
//void symtable_display_table();
void symtable_print_labels();

#endif