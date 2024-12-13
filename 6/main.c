 /****************************************
 * Unit Project 6 for CS 271
 * 
 * [NAME] Madison Neiswonger
 * [TERM] FALL 2024
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"
#include "hack.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTRUCTION_COUNT 30000


int main(int argc, const char *argv[]) {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        hashArray[i] = NULL; 
    }

    if (argc != 2) {
        exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
    }
    //take out later?
    printf("Input file: %s\n", argv[1]);


    FILE *file = fopen(argv[1], "r");
    if (!file) {
        exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
    } 

    instruction *instructions = malloc(MAX_INSTRUCTION_COUNT* sizeof(instruction));
    int instr_num = parse(file, instructions);

    symtable_print_labels();
    fclose(file);

    // (U6) Call Assemble Function:
	assemble(argv[1], instructions, instr_num);

    free(instructions);

    return 0;
}


