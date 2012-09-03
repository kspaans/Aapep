#!/bin/sh

TESTDIR="tests"
EXE="sexpr_test"

echo "Running tests..."

for f in `ls $TESTDIR/*.sexpr`; do
	BASE="`basename $f .sexpr`"
	echo -e "\t$BASE"
	./$EXE $f
	RETURN="$?"
	RETVAL="$TESTDIR/$BASE.retval"
	if [[ "$RETURN" != "`cat $RETVAL`" ]]; then
		echo -e "\tFAILED!"
		exit 1;
	fi
	echo -e "\t$BASE PASSED!"
done
