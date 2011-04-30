#ifndef NEBU_ARGV_H
#define NEBU_ARGV_H

#include <stdio.h>

typedef struct nebu_argv
{
	int nArguments;
	char **pKeys;
	char **pValues;
} nebu_argv;

nebu_argv* nebu_argv_Parse(int argc, const char *argv[]);
void nebu_argv_Debug_Print(nebu_argv *pArguments, FILE* output);

#endif
