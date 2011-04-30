#ifndef NEBU_FILE_SYSTEM_H
#define NEBU_FILE_SYSTEM_H

#include "base/nebu_util.h"
#include "filesystem/nebu_file_io.h"

typedef struct {
	int tag;
	int nDirs;
	const char **directories;
} nebu_FS_PathConfig;

void nebu_FS_ClearAllPaths(void);
void nebu_FS_SetupPath(int tag, int nDirs, const char **directories);
char* nebu_FS_GetPath(int tag, const char *filename);
int nebu_FS_Test(const char *path);

extern nebu_List* readDirectoryContents(const char *dirname, const char *prefix);

/* findpath.c, GPL'd code */
extern void goto_installpath(const char *executable);

extern void makeDirectory(const char* name);

#endif
