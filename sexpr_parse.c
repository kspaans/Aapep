/*
 * File for simply parsing S-Expressions into C structs
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

struct atom;

struct sexpr {
	struct atom *first;
	struct atom *rest;
};

enum atom_type {
	EMPTY,
	SEXPR,
	NUM,
};

struct atom {
	enum atom_type atom_t;
	union contents {
		struct sexpr *sexp;
		int num;
	} contents;
};

enum parse_state {
	SBOF,
	SEOF,
	SERROR,
	SOPEN,
	SCLOSE,

};

struct atom *lex_atom(FILE *f, unsigned c)
{
	struct atom *a;
	char num[11] = {};
	unsigned i = 0;

	if (isdigit(c)) {
		num[i] = c;
		i += 1;
		while (isdigit(c = fgetc(f))) {
			num[i] = c;
			i += 1;
		}
		if (11 == i) {
			fprintf(stderr, "ERROR: overflow\n");
		}
		num[10] = '\0';

		a = malloc(sizeof(*a));
		a->atom_t = NUM;
		a->contents.num = atoi(num);
	} else {
		fprintf(stderr, "ERROR: invalid char in atom.\n");
		return NULL;
	}

	return a;
}

int parse_sexpr(FILE *f, struct atom *current_atom, unsigned c, enum parse_state state)
{
	struct atom *new_atom;

	while (1) {
		switch (state) {
		case SBOF:
		//	printf("P\tSBOF\n");
			while (isspace(c = fgetc(f))) {}
		//	printf("P\tread 0x%02x\n", c);
			if        (c == '(') {
				state = SOPEN;
			} else if (c == EOF) { /* Don't accept empty files */
				state = SERROR;
			} else {
				state = SERROR;
			}
			break;

		case SERROR:
		//	printf("P\tSERROR\n");
			return 1;

		case SOPEN:
		//	printf("P\tSOPEN\n");
			current_atom->atom_t = SEXPR;
			current_atom->contents.sexp = malloc(sizeof(struct sexpr));
		//	printf("P\tallocated a sexpr at           %p\n", current_atom->contents.sexp);

			while (isspace(c = fgetc(f))) {}
			if ('(' == c) {
				current_atom->contents.sexp->first = malloc(sizeof(struct atom));
		//		printf("P\tallocated an atom for first at %p\n", current_atom->contents.sexp->first);
				parse_sexpr(f, current_atom->contents.sexp->first, c, SOPEN);
			} else if (')' == c) {
				current_atom->contents.sexp->first = NULL;
				current_atom->contents.sexp->rest = NULL;
				return 0;
			} else {
		//		printf("P\tFprobably some atom: 0x%02x\n", c);
				new_atom = lex_atom(f, c);
					current_atom->contents.sexp->first = new_atom;
				if (NULL != new_atom) {
				} else {
					fprintf(stderr, "Skipping atom, will continue parsing.\n");
				}
			}

			while (isspace(c = fgetc(f))) {}
			if ('(' == c) {
				current_atom->contents.sexp->rest = malloc(sizeof(struct atom));
		//		printf("P\tallocated an atom for rest  at %p\n", current_atom->contents.sexp->rest);
				parse_sexpr(f, current_atom->contents.sexp->rest, c, SOPEN);
			} else if (')' == c) {
				current_atom->contents.sexp->rest = NULL;
				return 0;
			} else {
		//		printf("P\tRprobably some atom: 0x%02x\n", c);
				current_atom->contents.sexp->rest = NULL;
			}

			while (isspace(c = fgetc(f))) {}
			if (')' != c) {
				fprintf(stderr, "PARSE ERROR: '%c' (0x%02x)\n", c, c);
				return 1;
			}
			return 0;

		default:
			goto done;
		};
		//break;
	} done:;

	return 0;
}

int sexpr_pprint(struct sexpr *s);

int atom_pprint(struct atom *a)
{
	if (NULL == a) {
		return 0;
	}

	switch (a->atom_t) {
	case EMPTY:
		return 0;
		break;

	case SEXPR:
		sexpr_pprint(a->contents.sexp);
		break;

	case NUM:
		printf("%d", a->contents.num);
		break;

	default:
		fprintf(stderr, "WOAH, WHAT?!\n");
		abort();
	}

	return 0;
}

int sexpr_pprint(struct sexpr *s)
{
	if (NULL == s) {
		return 0;
	}

	printf("(");
	atom_pprint(s->first);
	printf(" ");
	atom_pprint(s->rest);
	printf(")");

	return 0;
}

int main(int argc, char **argv)
{
	struct atom a;
	struct sexpr *s;
	FILE *f;
	int retval;

	if (argc != 2) return 1;

	f = fopen(argv[1], "r");

	retval = parse_sexpr(f, &a, '\0', SBOF);
	s = a.contents.sexp;
	//while (s) {
	//	printf("(");
	//}

//	/*
	#if 0
	printf("atom a at location          %p\n", &a);
	printf("a.atom_t at location        %p\n", &a.atom_t);
	printf("a.contents at location      %p\n", &a.contents);
	printf("a.contents.sexp at location %p\n", &a.contents.sexp);
	printf("a.contents.sexp points to   %p\n", a.contents.sexp);
	if (!a.contents.sexp) return retval;
	printf("a.contents.sexp.first " "   %p\n", a.contents.sexp->first);
	printf("a.contents.sexp.rest  " "   %p\n", a.contents.sexp->rest);
	#endif
//	*/

	atom_pprint(&a);
	printf("\n");

	return retval;
}
