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
#include <unistd.h>
#include <stdarg.h>
#include "arm.h"


void display_processor_state(arm_processor_state processor_state);
char **load_file(char *file, uint32_t *lines); //calls malloc
bool readfile(char *file);

int main() {
	arm_processor_state processor_state;
	memset(&processor_state, 0, sizeof(arm_processor_state));
	display_processor_state(processor_state);
	uint32_t mc;
	armv7_init_hash_map();
	uint32_t tmpin; 
	char **tmp = load_file("test.asm", &tmpin);
	processor_state.cpsr |= 1<<29;
	for (int i = 0; i < tmpin; i++) {
		mc = armv7_compile_line(tmp[i]);
		printf("--------------------------------------------------------------------------------\n");
		printf("%s\n", tmp[i]);
		armv7_execute_machine_code(&processor_state, mc);
		display_processor_state(processor_state);
		printf ("\n\n");
	}
}

bool readfile(char *file) {
	int fsize = 0, flines = 0;
	char *filebuff;
	char c;
	bool content = false;
	FILE *fptr = fopen(file, "r");
	if (fptr == NULL) return false;
	while ((c = fgetc(fptr)) != EOF) {
		if (c == ';' && !content) flines--;
		if (c == '\n') {
			flines++;
			content = false;
		}
		content == true;
		fsize++;
	}
	filebuff = malloc(fsize);
	rewind(fptr);
	fread(filebuff, 1, fsize, fptr);
	printf("%s", filebuff);
	fclose(fptr);
}

void display_processor_state(arm_processor_state processor_state) {
	printf("--------------------------------------------------------------------------------\n");
	printf("Current State\n");
	printf("Register\tHex\t\tDec\t\tBin\n");
	printf("CPSR\t\t%.8x\t%.10d\t%.32b\n", processor_state.cpsr, processor_state.cpsr, processor_state.cpsr);
	for (int i = 0; i < 16; i++) {
		printf("r%d\t\t%.8x\t%.10d\t%.32b\n", i, processor_state.r[i], processor_state.r[i], processor_state.r[i]);
	}
	printf("--------------------------------------------------------------------------------\n");
}

char **load_file(char *file, uint32_t *lines) {
	FILE *fptr = fopen(file, "r");
	if (!fptr) return NULL;
	char c; *lines = 0;
	while ((c = fgetc(fptr)) != EOF) {
		if (c == '\n') {
			*lines += 1;
		}
	}
	rewind(fptr);
	char **out = malloc(*lines * sizeof(char**));
	char buffer[32];
	size_t i = 0, n = 0;
	memset(buffer, 0, 32);
	while ((c = fgetc(fptr)) != EOF) {
		if (c == '\n') {
			out[n] = malloc((i + 1) * sizeof(char*));
			memset(out[n], 0, i + 1);
			strcpy(out[n], buffer);
			memset(buffer, 0, 32);
			i = 0;
			n++;
		} else {
			buffer[i] = c;
			i++;
		}
	}
	fclose(fptr);
	return out;
}
