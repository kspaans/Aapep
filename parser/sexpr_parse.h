#ifndef __SEXPR_PARSE_H__
#define __SEXPR_PARSE_H__

#include <stdio.h>

#define MAX_NAME_LEN    32
#define MAX_SYMBOL_LEN  32
#define MAX_STRING_LEN 256

struct atom;

struct sexpr {
	struct atom *first;
	struct atom *rest;
};

enum atom_type {
	EMPTY,
	SEXPR,
	NUM,
	SYMBOL,
	STRING,
	NAME,   /* Must not start with a number, ', or ". */
};

struct atom {
	enum atom_type atom_t;
	union contents {
		struct sexpr *sexp;
		int           num;
		const char   *symb;
		char         *str;
		const char   *name;
	} contents;
};

enum parse_state {
	SBOF,
	SEOF,
	SERROR,
	SOPEN,
	SCLOSE,

};

extern int atom_pprint(struct atom *a);
extern int parse_sexpr(FILE *f, struct atom *current_atom, unsigned c, enum parse_state state);

#endif
