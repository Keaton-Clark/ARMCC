#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool readFile(char* file, char** lines);

int main (int argc, char* argv[]) {
	if (argc != 2) {
	       printf("Incorrect number of arguments given.\n%d: Given\n1: Required\n", argc - 1);
	} else {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
				printf("\nUsage:\n%s [file or flag]\n\nFlags:\n-h or --help\tShow this message\n", argv[0]);
				return 0;
		} else {
			char* asmLines[100];
			if (!readFile(argv[1], asmLines)) {
				printf("'%s' not found or cannot be opened\n", argv[1]); 
				return -1;
			}
				
			printf("%s", asmLines[0]);
		}
	}

	return 0;
}

bool readFile(char* file, char** lines) {
	FILE* fptr;
	fptr = fopen(file, "r");
	if (fptr == NULL) {
		return false;
	}

	size_t i = 0;
	do { 
		lines[i] = (char*)malloc(100); 
		i++;
	} while (fgets(lines[i-1], 100, fptr) != NULL);
	fclose(fptr);

	return true;
}
