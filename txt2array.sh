#!/usr/bin/env bash
# read lines of text from given file and print it as a C string array
# WARNING: risk of irreversible damage to files if paired with >
# usage: ./txt2array.sh input_file
if [ $# -gt 0 ]
then
	printf 'char *dict[] = {\n'
	declare -i count=0
	while IFS= read -r line; do
	  printf '\t"%s",\n' "$line"
	  count=$(($count + 1))
	done < $1
	printf '};\n'
	printf 'int dictlen = %d;\n' "$count"
fi

