#ifndef __AAPEP_MACHINE_H__
#define __AAPEP_MACHINE_H__

/* First valid address is 0x1000 */
#define START_ADDRESS 0x1000

/*
 * Machine statistics, recorded whenever the machine is run.
 */
struct machine_stats {
	unsigned int ticks;
};

struct machine_state {
	unsigned int ip;
	unsigned int status;
	int registers[32];
	unsigned int memsize;
	unsigned int *memory;
	struct machine_stats stats;
};

enum err_t {
	OK = 0,
	GENERAL_ERROR = -1,
	BAD_MEMORY_ADDRESS = -2,
	NO_INPUT_FILE = -3,
	INPUT_ERROR = -4,
	INVALID_INSTRUCTION = -5,
};

#endif
