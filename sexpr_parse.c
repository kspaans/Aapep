/*
 * File for simply parsing S-Expressions into C structs
 */

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
};

struct atom {
	enum atom_type atom_t;
	union contents {
		struct sexpr *sexp;
	} contents;
};

enum parse_state {
	SBOF,
	SEOF,
	SERROR,
	SOPEN,
	SCLOSE,

};

int sexpr_pprint(struct sexpr *s);

int parse_sexpr(FILE *f, struct atom *current_atom)
{
	enum parse_state state = SBOF;
	char c = '\0';
	unsigned idx = 0;

	while (1) {
	//	printf("P\tLOOP %08d: state = %d\n", idx++, state);

		switch (state) {
		case SBOF:
		//	printf("P\tSBOF\n");
			c = fgetc(f);
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

		case SEOF:
		//	printf("P\tSEOF\n");
			return 1;

		case SOPEN:
		//	printf("P\tSOPEN\n");
			current_atom->atom_t = SEXPR;
			current_atom->contents.sexp = malloc(sizeof(struct sexpr));
		//	printf("P\tallocated a sexpr at %p\n", current_atom->contents.sexp);
			current_atom->contents.sexp->first = NULL;
			current_atom->contents.sexp->rest = NULL;

			c = fgetc(f);
		//	printf("P\tread 0x%02x\n", c);
			if        (c == ')') {
				state = SCLOSE;
			} else if (c == EOF) {
				state = SERROR;
			} else {
				state = SERROR;
			}
			break;

		case SCLOSE:
		//	printf("P\tSCLOSE\n");
			return 0;

		default:
			goto done;
		};
		//break;
	} done:;

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

	retval = parse_sexpr(f, &a);
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
