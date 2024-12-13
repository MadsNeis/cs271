/****************************************
 * Unit Project 6 for CS 271
 * 
 * [NAME] Madison Neiswonger
 * [TERM] FALL 2024
 * 
 ****************************************/

#include "hack.h"
#include "parser.h"
#include "symtable.h"
#include "error.h"
#include <string.h>	
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


 // Function Strip:
char* strip(char* s) {
	char s_new[MAX_LINE_LENGTH + 1] = { 0 };
	int i = 0;
	for (char* s2 = s; *s2; s2++) {
		if (*s2 == '/' && *(s2 + 1) == '/') {
			break;
		}
		else if (!isspace(*s2) || (i > 0 && !isspace(s_new[i - 1]))) {
			s_new[i++] = *s2;
		}
	}
	s_new[i] = '\0';
	strcpy(s, s_new);

	i = strlen(s) - 1;
	while (i >= 0 && isspace(s[i])) {
		s[i--] = '\0';
	}
	return s;
}

// Funtion: is_Atype:
bool is_Atype(const char* line) {
	return line[0] == '@';
}

// Function is_Ctype:
bool is_Ctype(const char* line) {
	return !is_Atype(line) && !is_label(line);
}


// Funcion is_label:
bool is_label(const char* line) {
	int len = strlen(line);
	return len > 2 && line[0] == '(' && line[len - 1] == ')';
}


// Function extract_label:
char* extract_label(const char* line, char* label) {
	int i = 0;
	for (const char* s = line; *s; s++) {
		if (*s == '(' || *s == ')') {
			continue;
		}
		else {
			label[i++] = *s;
		}
	}
	return label;
}
//char *extract_label(const char* line, char* label) {
//	int len = strlen(line);
//	if (len > 2 && line[0] == '(' && line[len - 1] == ')') {
//		strncpy(label, line + 1, len - 2);
//		label [len - 2] = '\0';
//	} else {
//		label[0] = '\0';
//	}
//	return label;
//}

//Function parse_A_instruction:
bool parse_A_instruction(const char* line, a_instruction* instr) {
	char* s = malloc(strlen(line));
	if (!s) {
		perror("Failed");
		return false;
	}

	strcpy(s, line + 1);

	char* s_end = NULL;
	long result = strtol(s, &s_end, 10);

	if (s == s_end) {
		instr->operand.label = (char*)malloc(strlen(line));
		strcpy(instr->operand.label, s);
		instr->is_addr = false;
	}
	else if (*s_end != '\0') {
		return false;
	}
	else {
		instr->operand.addr = (hack_addr)result;
		instr->is_addr = true;
	}
	return true;
}

//Function parse_C_instruction
void parse_C_instruction(char *line, c_instruction *instr) {

	char* temp = strtok(line, ";");
	char* jump = strtok(NULL, ";");

	char* dest = strtok(temp, "=");
	char* comp = strtok(NULL, "=");

	if (comp == NULL) {
		comp = dest;
		dest = NULL;
	}

	instr->jump = str_to_jumpid(jump);
	int a;
	instr->comp = str_to_compid(comp, &a);
	instr->a = (comp[0] == 'M') ? 1 : 0;
	instr->dest = (dest != NULL) ? str_to_destid(dest) : DEST_NULL;
}


//Assemble Function:

void assemble(const char* file_name, instruction* instructions, int instr_num) {
	char output_file_name[256];
	char* base_name = strrchr(file_name, '/');
	if (!base_name) base_name = strrchr(file_name, '\\');
	base_name = base_name ? base_name + 1 : file_name; 

	char* dot = strrchr(base_name, '.'); 
	if (dot && strcmp(dot, ".asm") == 0) {
		snprintf(output_file_name, sizeof(output_file_name), "%.*s.hack", (int)(dot - base_name), base_name);
	}
	else {
		snprintf(output_file_name, sizeof(output_file_name), "%s.hack", base_name);
	}

	FILE* output_file = fopen(output_file_name, "w");
	if (!output_file) {
		perror("Failed to open output file");
		exit(EXIT_FAILURE);
	}

	int next_variable_address = 16;

	for (int i = 0; i < instr_num; i++) {
		instruction instr = instructions[i];
		opcode op = 0;

		if (instr.itype == Atype) {
			if (instr.instr.a.is_addr) {
				op = instr.instr.a.operand.addr;
			}
			else {
				Symbol* symbol = symtable_find(instr.instr.a.operand.label);
				int address = symbol ? symbol->addr : -1;
				if (address == -1) {
					address = next_variable_address++;
					symtable_insert(instr.instr.a.operand.label, address);
				}
				op = address;
			}
		}
		else if (instr.itype == Ctype) {
			op = instruction_to_opcode(instr.instr.c);
		}
		fprintf(output_file, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", OPCODE_TO_BINARY(op));
	}

	printf("Finished writing to file: %s\n", output_file_name);
	fclose(output_file);
}




void add_predefined_symbols() {
	for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++) {
		struct predefined_symbol symbol = predefined_symbols[i];
		symtable_insert(symbol.name, symbol.address);
	}
}

// (U6) instruction_to_opcode Function:
opcode instruction_to_opcode(c_instruction instr){
	opcode op = 0;
	op |= (7 << 13);

	op |= (instr.a << 12);
	op |= (instr.comp << 6);
	op |= (instr.dest << 3);
	op |= instr.jump;

	return op;
}

// Function Parse:
int parse(FILE* file, instruction* instructions) {
	char line[MAX_LINE_LENGTH] = { 0 };
	unsigned int line_num = 0;
	unsigned instr_num = 0;

	instruction instr;

	add_predefined_symbols();

	//symtable_display_table();

	while (fgets(line, sizeof(line), file)) {
		line_num++;
		strip(line);

		if (!*line) {
			continue;
		}

		char inst_type = ' ';

		//printf("line: %s \n", line);

		if (instr_num >= MAX_INSTRUCTIONS) {
			exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
		}
		if (is_label(line)) {
			char label[MAX_LABEL_LENGTH] = { 0 };
			strcpy(line, extract_label(line, label));
			/*if (label[0] == '\0') {
				exit_program(EXIT_INVALID_LABEL, line_num, line);
			}*/
			if (!isalpha(label[0])) {
				exit_program(EXIT_INVALID_LABEL, line_num, label);
			}
			if (symtable_find(label) != NULL) {
				exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, label);
			}

			symtable_insert(label, instr_num);
			continue;
		}
		else if (is_Atype(line)) {
			inst_type = 'A';
			if (!parse_A_instruction(line, &instr.instr.a)) {
				exit_program(EXIT_INVALID_A_INSTR, line_num, line);
			}
			instr.itype = Atype;

			if (instr.instr.a.is_addr) {
				printf("A: %d\n", instr.instr.a.operand.addr);
			}
			else {
				printf("A: %s\n", instr.instr.a.operand.label);
			}
		}
		else if (is_Ctype(line)) {
			char tmp_line[MAX_LINE_LENGTH];
			strcpy(tmp_line, line);
			parse_C_instruction(tmp_line, &instr.instr.c);

			if (instr.instr.c.dest == DEST_INVALID) {
				exit_program(EXIT_INVALID_C_DEST, line_num, line);
			}
			if (instr.instr.c.comp == COMP_INVALID) {
				exit_program(EXIT_INVALID_C_COMP, line_num, line);
			}
			if (instr.instr.c.jump == JMP_INVALID) {
				exit_program(EXIT_INVALID_C_JUMP, line_num, line);
			}

			instr.itype = Ctype;

			printf("C: d=%d, c=%d, j=%d\n", instr.instr.c.dest, instr.instr.c.comp, instr.instr.c.jump);
		}
		//printf("%u: %c  %s\n", instr_num, inst_type, line);
		instructions[instr_num++] = instr;
	}
	return instr_num;
}