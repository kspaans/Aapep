/*
 * The ... virtual machine
 */

#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "machine.h"
#include "opcodes.h"

enum err_t decode_and_run(struct machine_state *m, char *filename)
{
	FILE *f;
	size_t err;
	int reg_src1;
	int reg_src2;
	int reg_dst;
	enum err_t ierr;
	uint32_t instr;
	enum opcode_t op;

	f = fopen(filename, "r");
	/* TODO check return value */

	while (1) {
		err = fread(&instr, 4, 1, f);
		if (1 != err) {
			printf("ERROR: fread returned %d, expected 1\n", err);
			return INPUT_ERROR;
		}

		printf("read   %08X\n", instr);
		op = GET_OPCODE(instr);
		printf("opcode %08X\n", op);
		switch (op) {
		case OP_ADD:
			ierr = add(m, GET_SRC1(instr), GET_SRC2(instr), GET_DST(instr));
			if (OK != ierr) {
				return ierr;
			}
			break;

		case OP_ADD_IMM:
			ierr = addi(m, GET_SRC1(instr), GET_IMM(instr));
			if (OK != ierr) {
				return ierr;
			}
			break;

		default:
			printf("ERROR: invalid opcode: 0x%08X\n", op);
			return INVALID_INSTRUCTION;
		}
	}

	return OK;
}

void initialize_machine(struct machine_state *m)
{
	for (int i = 0; i < 32; i += 1) {
		m->registers[i] = 0x0;
	}
	m->status = 0x0;
	m->ip =     START_ADDRESS;

	m->stats.ticks = 0;
}

void print_machine_state(struct machine_state *m)
{
	printf("Ticks: %u  IP: 0x%08X  Status: 0x%08X\n", m->stats.ticks, m->ip,
		m->status);
	for (int i = 0; i < 32; i += 4) {
		printf("R%02d: 0x%08X  R%02d: 0x%08X  R%02d: 0x%08X  R%02d: 0x%08X\n",
			i, m->registers[i], i+1, m->registers[i+1], i+2,
			m->registers[i+2], i+3, m->registers[i+3]);
	}
}

int main(int argc, char **argv)
{
	struct machine_state m;
	enum err_t retval;

	if (argc < 2) {
		printf("ERROR: no input file\n");
		return NO_INPUT_FILE;
	}

	initialize_machine(&m);
	m.memsize = 0x10000;
	m.memory =  malloc(0x10000);

	retval = decode_and_run(&m, argv[1]);

	print_machine_state(&m);
	return retval;
}
