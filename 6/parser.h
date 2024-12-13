/****************************************
 * Unit Project 6 for CS 271
 * 
 * [NAME] Madison Neiswonger
 * [TERM] FALL 2024
 * 
 ****************************************/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "hack.h"
#include "symtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH (MAX_LINE_LENGTH - 2)
#define MAX_HACK_ADDRESS (INT16_MAX)
#define MAX_INSTRUCTIONS (MAX_HACK_ADDRESS)

typedef int16_t hack_addr;
typedef int16_t opcode; 

char *strip(char *s);

bool is_Atype(const char *s);

bool is_label(const char *s);

bool is_Ctype(const char *s);


char* extract_label(const char *line, char *label);

typedef enum instr_type {
	Invalid = -1,
	Atype,
	Ctype,
} instr_type;

typedef struct c_instruction {
	opcode a : 1;
	opcode comp : 7;
	opcode dest : 4;
	opcode jump : 4;
} c_instruction;

typedef struct a_instruction {
	union {
		hack_addr addr;
		char *label;
	} operand;
	bool is_addr; 
}a_instruction;

typedef struct instruction {
	enum instr_type itype;
	union {
		c_instruction c;
		a_instruction a;
	}instr;
} instruction;

bool parse_A_instruction(const char* line, a_instruction* instr);

void add_predefined_symbols();

void parse_C_instruction(char *line, c_instruction *instr);

void assemble(const char* file_name, instruction* instructions, int instr_num);

int parse(FILE * file, instruction *instructions); 



opcode instruction_to_opcode(c_instruction instr);

#endif