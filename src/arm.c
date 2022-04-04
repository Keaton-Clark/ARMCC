#include "arm.h"
#include "hash.h"

#define ISDELIM(CHAR) (CHAR == ' ' || CHAR == '\t' || CHAR == ',')
#define ISNUM(CHAR) (CHAR >= '0' && CHAR <= '9' || CHAR == 'X' || CHAR >= 'A' && CHAR <= 'F')

#define NSET(CPSR) (CPSR & (1<<31))
#define ZSET(CPSR) (CPSR & (1<<30))
#define CSET(CPSR) (CPSR & (1<<29))
#define VSET(CPSR) (CPSR & (1<<28))

#define OPERAND2(INSTR) 


static char *Regs[] = {
	"R0", "R1", "R2", "R3", 
	"R4", "R5", "R6", "R7", 
	"R8", "R9", "R10", "R11", 
	"R12", "R13", "R14", "R15"};
static char *Conds[] = {
	"EQ", "NE", "CS", "CC", "MI",
	"PL", "VS", "VC", "HI", "LS",
	"GE", "LT", "GT", "LE", "AL"};
static char *OpCodes[] = {
	"AND", "EOR", "SUB", "RSB", 
	"ADD", "ADC", "SBC", "RSC", 
	"TST", "TEQ", "CMP", "CMN", 
	"ORR", "MOV", "BIC", "MVN"};

typedef union {
	struct {
		uint32_t Cond : 4;
		uint32_t nil : 2;
		uint32_t I : 1;
		uint32_t OpCode : 4;
		uint32_t S : 1;
		uint32_t Rn : 4;
		uint32_t Rd : 4;
		uint32_t Shift : 8;
		uint32_t Rm : 4;
	} dataprocessingRm;
	struct {
		uint32_t Cond : 4;
		uint32_t nil : 2;
		uint32_t I : 1;
		uint32_t OpCode : 4;
		uint32_t S : 1;
		uint32_t Rn : 4;
		uint32_t Rd : 4;
		uint32_t Rotate : 4;
		uint32_t Imm : 8;
	} dataprocessingImm;
	uint32_t bits;
} armv7_instr;

uint8_t arm_readnumber (char *str, uint32_t *result) {
	char buff[32];
	memset(buff, 0, 16);
	for (int i = 0; ISNUM(*str); i++, str++) {
		buff[i] = *str;
	}
	*result = strtol(buff, NULL, 0);
	return 1;
}

uint32_t armv7_compile_line(char *str) {
	upper(str);
	armv7_instr instr;
	uint32_t tmp;
	memset(&instr, 0, 4);
	while (!hash_search(str, str + 2, &tmp)) str++;
	if (tmp <= 15) {
		instr.dataprocessingRm.OpCode = tmp;
	} else {
		switch (tmp) {
			case 17 : //ASR
				instr.bits |= 1<<25;
				instr.bits |= 1<<24;
				break;
			case 18 : //LSR
				instr.bits |= 1<<26;
				instr.bits |= 1<<24;
				break;
			case 19 : //LSL
				instr.bits |= 1<<24;
				break;
		}
		instr.dataprocessingRm.OpCode = 13;
	}
	str += 3;
	if (*str == 'S') {
		instr.dataprocessingRm.S = 1; 
		str++;
	}
	instr.dataprocessingRm.Cond = 14;
	if (hash_search(str, str + 1, &tmp)) {
		instr.dataprocessingRm.Cond = tmp;
		str += 2;
	}
	while (ISDELIM(*str)) str++;
	if (hash_search(str, str+1, &tmp)) {
		instr.dataprocessingRm.Rd = tmp;
		str += 2;
	}
	while (ISDELIM(*str)) str++;
	if (hash_search(str, str+1, &tmp)) {
		instr.dataprocessingRm.Rn = tmp;
		str += 2;
	} else if (*str == '#') {
		str++;
		if (arm_readnumber(str, &tmp)) {
			instr.dataprocessingImm.I = 1;
			instr.dataprocessingImm.Imm = tmp;
		}
		return instr.bits;
	}
	while (ISDELIM(*str)) str++;
	if (hash_search(str, str+1, &tmp)) {
		instr.dataprocessingRm.Rm = tmp;
	} else if (*str == '#') {
		str++;
		if (arm_readnumber(str, &tmp)) {
			instr.dataprocessingImm.I = 1;
			instr.dataprocessingImm.Imm = tmp;
		}
		return instr.bits;
	}
	return instr.bits;
}

void armv7_init_hash_map() {
	for (int i = 0; i < 16; i++) 
		hash_add(OpCodes[i], i);
	for (int i = 0; i < 15; i++)
		hash_add(Conds[i], i);
	for (int i = 0; i < 16; i++)
		hash_add(Regs[i], i);
	hash_add("ASR", 17);
	hash_add("LSR", 18);
	hash_add("LSL", 19);
}

char *arm_decompile_machine_code(uint32_t machine_code) {
	char *out = malloc(128);
	memset(out, 0, 128);
	armv7_instr instr; 
	instr.bits = machine_code;
	strcat(out, OpCodes[instr.dataprocessingRm.OpCode]);
	if (instr.dataprocessingRm.S) strcat(out, "S");
	strcat(out, Conds[instr.dataprocessingRm.Cond]);
	strcat(out, "\t");
	strcat(out, Regs[instr.dataprocessingRm.Rd]);
	return out;
}

bool armv7_execute_conditional (uint32_t cpsr, uint8_t Cond) {
	switch (Cond) {
		case 0 :
			if (ZSET(cpsr)) return true;
			break;
		case 1 :
			if (!ZSET(cpsr)) return true;
			break;
		case 2 :
			if (CSET(cpsr)) return true;
			break;
		case 3 :
			if (!CSET(cpsr)) return true;
			break;
		case 4 :
			if (NSET(cpsr)) return true;
			break;
		case 5 :
			if (!NSET(cpsr)) return true;
			break;
		case 6 :
			if (VSET(cpsr)) return true;
			break;
		case 7 :
			if (!VSET(cpsr)) return true;
			break;
		case 8 :
			if (CSET(cpsr) && !ZSET(cpsr)) return true;
			break;
		case 9 :
			if (!CSET(cpsr) || ZSET(cpsr)) return true;
			break;
		case 10 :
			if (NSET(cpsr) == VSET(cpsr)) return true;
			break;
		case 11 :
			if (NSET(cpsr) != VSET(cpsr)) return true;
			break;
		case 12 :
			if (!ZSET(cpsr) && (NSET(cpsr) == VSET(cpsr))) return true;
			break;
		case 13 :
			if (ZSET(cpsr) || (NSET(cpsr) != VSET(cpsr))) return true;
			break;
		case 14 :
			return true;
			break;
	}
	return false;
}

void armv7_execute_machine_code(arm_processor_state *processor_state, uint32_t machine_code) {
	armv7_instr instr;
	instr.bits = machine_code;
	if (!armv7_execute_conditional(processor_state->cpsr, instr.dataprocessingRm.Cond)) return;
	switch (instr.dataprocessingRm.OpCode) {
		case 0 : //AND
			processor_state->r[instr.dataprocessingRm.Rd] = 
				processor_state->r[instr.dataprocessingRm.Rn] &
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]);
			return;
		case 1 : //EOR
			processor_state->r[instr.dataprocessingRm.Rd] = 
				processor_state->r[instr.dataprocessingRm.Rn] ^
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]);
			return;
		case 2 : //SUB
			processor_state->r[instr.dataprocessingRm.Rd] =
				processor_state->r[instr.dataprocessingRm.Rn] -
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]);
			return;
		case 3 : //RSB
			processor_state->r[instr.dataprocessingRm.Rm] =
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]) -
				processor_state->r[instr.dataprocessingRm.Rn];
			return;
		case 4 : //ADD
			processor_state->r[instr.dataprocessingRm.Rd] =
				processor_state->r[instr.dataprocessingRm.Rn] +
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]);
			return;
		case 5 : //ADC
			processor_state->r[instr.dataprocessingRm.Rd] =
				processor_state->r[instr.dataprocessingRm.Rn] +
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]) +
				(CSET(processor_state->cpsr)>>29) - 1;
			return;
		case 6 : //SBC
			processor_state->r[instr.dataprocessingRm.Rd] =
				processor_state->r[instr.dataprocessingRm.Rn] -
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]) +
				(CSET(processor_state->cpsr)>>29);
			return;
		case 12 : //ORR
			processor_state->r[instr.dataprocessingRm.Rd] = 
				processor_state->r[instr.dataprocessingRm.Rn] |
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]);
			return;
		case 13 : //MOV
			processor_state->r[instr.dataprocessingRm.Rd] =
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rn]);
				if (instr.bits & (1<<24) && !instr.dataprocessingRm.I) {
					if (instr.bits & 1<<25 || instr.bits & 1<<26) {
						processor_state->r[instr.dataprocessingRm.Rd] >>= 1;
					} else {
						processor_state->r[instr.dataprocessingRm.Rd] <<= 1;
					}

				}
			return;
		case 14 : //BIC
			processor_state->r[instr.dataprocessingRm.Rd] = 
				processor_state->r[instr.dataprocessingRm.Rn] &
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rm]);
			return;
		case 15 : //MVN
			processor_state->r[instr.dataprocessingRm.Rd] =
				(instr.dataprocessingRm.I ? instr.dataprocessingImm.Imm : processor_state->r[instr.dataprocessingRm.Rn]);
			return;
	}
}
