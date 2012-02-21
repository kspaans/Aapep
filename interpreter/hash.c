/*
 * Hash and hash table functionality.
 */

#include <string.h>
#include "hash.h"
#include "sexpr_parse.h"

static void *table[TABLE_SIZE];

uint64_t hash(char *str)
{
	uint64_t hashval = 5381;
	int c;

	while ((c = *str++))
	hashval = ((hashval << 5) + hashval) + c;

	return hashval;
}

void hash_init(void)
{
	int i;

	for (i = 0; i < TABLE_SIZE; i += 1) {
		table[i] = NULL;
	}
}

int hash_ins(char *k, struct atom *v)
{
	uint64_t h;
	struct hash_entry *ins;

	h = hash(k);

	return 0;
}

int hash_get(char *k, struct atom **v)
{
	uint64_t h1, h2;
	struct hash_entry *get;

	h1 = h2 = hash(k);
	get = table[h1 % TABLE_SIZE];
	if (NULL == get) {
		return 1;
	}
	while (!strncmp(get->key, k, MAX_NAME_LEN)) {
		h2 += 1;
		if (h2 == h1) {
			return 1;
		}
		get = table[h2 % TABLE_SIZE];
	}
	*v = get->val;

	return 0;
}
