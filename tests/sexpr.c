#include <stdio.h>
#include "sexpr_parse.h"

int main(int argc, char **argv)
{
	struct atom a;
	FILE *f;
	int retval;

	if (argc != 2) return 1;

	f = fopen(argv[1], "r");

	retval = parse_sexpr(f, &a, '\0', SBOF);
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
