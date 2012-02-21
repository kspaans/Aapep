/*
 * Hash and hash table functionality.
 */

#ifndef __HASH_H__
#define __HASH_H__

#include <stdio.h>
#include <stdlib.h>
#include "sexpr_parse.h"

#define TABLE_SIZE 89

#if 0
#define HASH_GET_CHECKED(k, v) \
	if (0 != hash_get(k, v)) { \
		fprintf(stderr, "ERROR: hash_get failed."); \
		abort(); \
	}
#endif

struct hash_entry {
	char          key[MAX_NAME_LEN];
	struct atom  *val;
};

uint64_t hash(char *str);
int hash_ins(char *k, struct atom *v);
int hash_get(char *k, struct atom **v);

#endif
