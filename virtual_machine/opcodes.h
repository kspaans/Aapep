#ifndef __AAPEP_OPCODES_H__
#define __AAPEP_OPCODES_H__

#define OPCODE_MASK    (0xf8000000)
#define OPCODE_SHIFT   (27)
#define REG_SRC1_MASK  (0x07C00000)
#define REG_SRC1_SHIFT (22)
#define REG_SRC2_MASK  (0x003E0000)
#define REG_SRC2_SHIFT (17)
#define REG_DST_MASK   (0x0001F000)
#define REG_DST_SHIFT  (12)
#define IMM_MASK       (0x003FFFFF)

#define GET_x(instr, mask, shift) ((instr & mask) >> shift)
#define GET_OPCODE(i) GET_x(i, OPCODE_MASK, OPCODE_SHIFT)
#define GET_SRC1(i)   GET_x(i, REG_SRC1_MASK, REG_SRC1_SHIFT)
#define GET_SRC2(i)   GET_x(i, REG_SRC2_MASK, REG_SRC2_SHIFT)
#define GET_DST(i)    GET_x(i, REG_DST_MASK,  REG_DST_SHIFT)
#define GET_IMM(i)    GET_x(i, IMM_MASK, 0)

enum opcode_t {
	OP_ADD =     0x00, /* 00000 add */
	OP_ADD_IMM = 0x01, /* 00001 add immediate */
};

#endif
