//Name: Keaton Clark
//Class: CS 219.1001
//Title: Programming Project 1
//Date: 1/25/22
//Purpose: ASM interpreter written in C
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

bool readFile(const char* file, char** lines);
uint32_t evalAsmLn(char* asmLn);

int main (int argc, char* argv[]) {
	if (argc == 2) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
				printf("\nUsage:\n%s [file or flag]\n\nFlags:\n-h or --help\tShow this message\n", argv[0]);
				return 0;
		} else {
			char* asmLines[6];
			if (!readFile(argv[1], asmLines)) {	//checks if the file is open then reads it to and array of strings so I can parse them line by line
				printf("'%s' not found or cannot be opened\n", argv[1]); 
				return -1;
			}
			for (size_t i = 0; i < sizeof(asmLines)/sizeof(asmLines[0])-1; i++) {	//loops through the array of strings and evaluates each line
				printf("%s", asmLines[i]);
				printf("%x\n---------------------------\n", evalAsmLn(asmLines[i]));
			}
		}
	} else {
		printf("Incorrect number of arguments given.\n%d: Given\n1: Required\n", argc - 1);
		return -1;
	}
	return 0;
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
	if (strcmp(op, "ADD") == 0) { 
		return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0) //cool little "one-liner" that gets each of the two substrings with strtok, converts them to longs with strtol, and then masks them as uint32_t and finally adds them
			+
			(uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0);
	} else if (strcmp(op, "SUB") == 0) {
		return (uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0)
			-
			(uint32_t)strtol(strtok_r(NULL, " ", &tmp), NULL, 0);
	}	
	else {
		return 0;
	} 
}
