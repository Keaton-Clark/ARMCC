#ifndef COMP_H
#define COMP_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	uint32_t cpsr;
	uint32_t r[16];

} arm_processor_state;
char *arm_decompile_machine_code(uint32_t machine_code); //don't forget to free the pointer that is returned
uint32_t armv7_compile_line(char *str);
void armv7_init_hash_map();
void armv7_execute_machine_code(arm_processor_state processor_state, uint32_t machine_code);

#endif //COMP_H
