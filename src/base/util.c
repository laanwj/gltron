#include "filesystem/path.h"
#include "Nebu_scripting.h"

#include <stdio.h>

#include "base/nebu_debug_memory.h"

void runScript(int ePath, const char *name) {
    char *s;
    s = getPath(ePath, name);
	fprintf(stderr, "[script] start running script '%s'\n", name);
    scripting_RunFile(s);
	fprintf(stderr, "[script] finished running script '%s'\n", name);
    free(s);
}

