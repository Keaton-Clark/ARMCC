//Name: Keaton Clark
//Class: CS 219.1001
//Title: Programming Project
//Date: 1/25/22
//Purpose: ARM ASM interpreter written in C
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define ADD 0xb87d06e
#define AND 0xb87d1b8
#define ASR 0xb87d26b
#define LSR 0xb880136
#define LSL 0xb880130
#define NOT 0xb880936
#define ORR 0xb880dd8
#define SUB 0xb881f2f
#define XOR 0xb8833be

int32_t r[16];
int32_t cpsr;

typedef struct instr instr;
struct instr {
	uint32_t op;
	int32_t *opd;
	int32_t numopd;
};

bool readFile(const char* file, char** lines);
uint32_t evalAsmLn(char* asmLn);
uint32_t hash(char* str);
void gensample();

int main (int argc, char* argv[]) {
	if (argc == 2) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
				printf("\nUsage:\n%s [file or flag]\n\nFlags:\n-h or --help\tShow this message\n", argv[0]);
				return 0;
		} else if (strcmp(argv[1], "-g") == 0) {
			gensample();
			return 0;
		} else {
			char* asmLines[0xFF];
			if (!readFile(argv[1], asmLines)) {	//checks if the file is open then reads it to and array of strings so I can parse them line by line
				printf("'%s' not found or cannot be opened\n", argv[1]); 
				return -1;
			}
			for (size_t i = 0; i < sizeof(asmLines)/sizeof(asmLines[0])-1; i++) {	//loops through the array of strings and evaluates each line
				printf("%s", asmLines[i]);
				printf("0x%X\n---------------------------\n", evalAsmLn(asmLines[i]));
			}
		}
	} else {
		printf("Incorrect number of arguments given.\n%d: Given\n1: Required\n", argc - 1);
		return -1;
	}
	return 0;
}

uint32_t hash(char* str) { //Why? Because it looked fun, no good reason. Makes it slightly faster with less modularity
	int hash = 5381;
	int c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c;
	return hash;
}

bool readFile(const char* file, char** lines) {
	FILE* fptr = fopen(file, "r");
	if (fptr == NULL) {
		return false;
	}
	size_t bufsize = 32;
	char* buffer = (char*)malloc(bufsize);
	for (size_t i = 0; fgets(buffer, bufsize, fptr) != NULL; i++) {
		lines[i] = (char*)malloc(strlen(buffer));
		strcpy(lines[i], buffer);
	}
	free(buffer);
	fclose(fptr);
	return true;
}

uint32_t evalAsmLn(char* asmLn) {
	//using strtok_r for safety
	char* tmp = asmLn;
	char* op = strtok_r(asmLn, " ", &tmp);
	switch (hash(op)) {
		case ADD:
			return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0) //cool little "one-liner" that gets each of the two substrings with strtok, converts them to longs with strtol, and then masks them as uint32_t and finally adds them
				+
				(uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0);
		case ASR:
			return (uint32_t)((int32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0) >> 1);
		case LSR:
			return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0) >> 1;
		case LSL:
			return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0) << 1;
		case NOT:
			return ~(uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0);
		case ORR:
			return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0)
				|
				(uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0);
		case SUB:
			return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0)
				-
				(uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0);
		case XOR:
			return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0)
				^
				(uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0);
	}
}

char* oplist[] = {
	"ADD",
	"AND",
	"ASR",
	"LSR",
	"LSL",
	"NOT",
	"ORR",
	"SUB",
	"XOR",
};

void gensample() {
	srand(time(0));
	FILE* fileptr = fopen("samples/sample.txt", "w");
	if (!fileptr) return;
	for (int i = 0; i < 0xFF; i++) {
		fprintf(fileptr, "%s 0x%X 0x%X\n", oplist[rand()%9], rand(), rand());
	}
	fclose(fileptr);
}
