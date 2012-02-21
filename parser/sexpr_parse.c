/*
 * File for simply parsing S-Expressions into C structs
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "sexpr_parse.h"

/*
 * Names have enough exceptions that they are worth their own function, namely
 * being able to include operators and special built-ins.
 */
static struct atom *parse_name(FILE *f, unsigned c)
{
	struct atom *a;
	char *name;
	int i = 0;

	name = malloc(MAX_NAME_LEN);
	name[i] = c;
	i += 1;
	while (1) {
		c = fgetc(f);
		if (EOF == c) {
			return NULL;
		} else if ('(' == c || ')' == c) {
			fprintf(stderr, "ERROR: read parenthesis in name.");
			return NULL;
		} else if (isspace(c)) {
			name[i] = '\0';
			break;
		} else {
			name[i] = c;
			i += 1;
		}

		if (MAX_NAME_LEN == i) {
			name[i - 1] = '\0';
			break;
		}
	}

	a = malloc(sizeof(*a));
	a->atom_t = NAME;
	a->contents.name = name;

	return a;
}

static struct atom *lex_atom(FILE *f, unsigned c)
{
	char num[11] = {};
	struct atom *a;
	char *s;           /* bytestring for strings, symbols, etc... */
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
	} else if ('\'' == c) {
		s = malloc(MAX_SYMBOL_LEN);
		while (!isspace(c = fgetc(f))) {
			s[i] = c;
			i += 1;
			if (MAX_SYMBOL_LEN == i) {
				s[i - 1] = '\0';
				fprintf(stderr, "ERROR: symbol too long.\n");
				break;
			}
		}

		a = malloc(sizeof(*a));
		a->atom_t = SYMBOL;
		a->contents.symb = s;
	} else if ('\"' == c) {
		s = malloc(MAX_STRING_LEN);
		while ('\"' != (c = fgetc(f))) {
			if (EOF == c) {
				fprintf(stderr,
					"ERROR: unterminated string.\n");
				return NULL;
			}
			s[i] = c;
			i += 1;
			if (MAX_STRING_LEN == i) {
				s[i - 1] = '\0';
				fprintf(stderr, "ERROR: string too long.\n");
				break;
			}
		}

		a = malloc(sizeof(*a));
		a->atom_t = STRING;
		a->contents.str = s;
	} else {
		a = parse_name(f, c);
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

static int sexpr_pprint(struct sexpr *s)
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

	case SYMBOL:
		printf("\'%s", a->contents.symb);
		break;

	case STRING:
		printf("\"%s\"", a->contents.str);
		break;

	case NAME:
		printf("%s", a->contents.name);
		break;

	default:
		fprintf(stderr, "WOAH, WHAT?!\n");
		abort();
	}

	return 0;
}
