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

bool readfile(char *file);

int main() {
	arm_processor_state processor_state;
	memset(&processor_state, 0, sizeof(arm_processor_state));
	processor_state.cpsr = 1 << 31;
	display_processor_state(processor_state);
	char str[] = "MOVCS r7, #0xFF";
	uint32_t out;
	armv7_init_hash_map();
	uint32_t mc = armv7_compile_line(str);
	printf("%s\n", str);
	printf("%.32b\n%s", mc, arm_decompile_machine_code(mc));
	armv7_execute_machine_code(processor_state, mc);
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
