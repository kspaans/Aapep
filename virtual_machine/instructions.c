#include <stdio.h>
#include "instructions.h"
#include "machine.h"

enum err_t add(struct machine_state *m, int src1, int src2, int dst)
{
	printf("ADD %d, %d, %d\n", src1, src2, dst);
	m->registers[dst] = m->registers[src1] + m->registers[src2];

	m->ip += 4;
	m->stats.ticks += 1;

	return OK;
}

enum err_t addi(struct machine_state *m, int src1, int imm)
{
	printf("ADDI %d, %d\n", src1, imm);
	m->registers[src1] = imm;

	m->ip += 4;
	m->stats.ticks += 1;

	return OK;
}

enum err_t sub(struct machine_state *m, int src1, int src2, int dst)
{
	printf("SUB %d, %d, %d\n", src1, src2, dst);
	m->registers[dst] = m->registers[src1] - m->registers[src2];

	m->ip += 4;
	m->stats.ticks += 1;

	return OK;
}

enum err_t xor(struct machine_state *m, int src1, int src2, int dst)
{
	printf("XOR %d, %d, %d\n", src1, src2, dst);
	m->registers[dst] = m->registers[src1] ^ m->registers[src2];

	m->ip += 4;
	m->stats.ticks += 1;
}

enum err_t cons(struct machine_state *m, int car, int cdr)
{
	printf("CONS %d, %d\n", car, cdr);

	return OK;
}
