#ifndef __AAPEP_INSTRUCTIONS_H__
#define __AAPEP_INSTRUCTIONS_H__

#include "machine.h"

enum err_t add(struct machine_state *m, int src1, int src2, int dst);
enum err_t addi(struct machine_state *m, int src1, int imm);
enum err_t sub(struct machine_state *m, int src1, int src2, int dst);
enum err_t xor(struct machine_state *m, int src1, int src2, int dst);
enum err_t cons(struct machine_state *m, int car, int cdr);

#endif
