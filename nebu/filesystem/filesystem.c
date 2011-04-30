#include "filesystem/nebu_filesystem.h"
#include "base/nebu_util.h"

#include <stdio.h>
#include <string.h>
#include "base/nebu_assert.h"

#include "base/nebu_debug_memory.h"

int nebu_FS_Test(const char *path) {
  FILE *f = fopen(path, "r");
  if(f) {
    fclose(f);
    return 1;
  }
  return 0;
}

nebu_List *path_list = NULL;

void nebu_FS_ClearAllPaths(void)
{
	nebu_List *p;
	if(!path_list)
		return;

	for(p = path_list; p->next; p = p->next)
	{
		free(p->data);
	}
	nebu_List_Free(path_list);
}

void nebu_FS_SetupPath(int tag, int nDirs, const char **directories)
{
	nebu_List *p;

	if(path_list == NULL)
	{
		path_list = (nebu_List*) malloc(sizeof(nebu_List));
		path_list->next = NULL;
	}

	for(p  = path_list; p->next != 0; p = p->next)
	{
		nebu_FS_PathConfig *pConfig = (nebu_FS_PathConfig*) p->data;
		if(pConfig->tag == tag)
		{
			// replace directories
			// TODO: consider memory management
			pConfig->nDirs = nDirs;
			pConfig->directories = directories;
			return;
		}
	}
	p->next = malloc(sizeof(nebu_List));
	p->next->next = NULL;
	p->data = malloc(sizeof(nebu_FS_PathConfig));
	// append directories
	{
		nebu_FS_PathConfig *pConfig = (nebu_FS_PathConfig*) p->data;
		pConfig->tag = tag;
		pConfig->nDirs = nDirs;
		pConfig->directories = directories;
	}
}

char* nebu_FS_GetPath(int tag, const char *filename)
{
	nebu_List *p;
	for(p = path_list; p->next != 0; p = p->next)
	{
		nebu_FS_PathConfig *pConfig = (nebu_FS_PathConfig*) p->data;
		if(pConfig->tag == tag)
		{
			int i;
			for(i = 0; i < pConfig->nDirs;i++)
			{
				int length = strlen(pConfig->directories[i]) + 1 /* seperator */ + strlen(filename) + 1 /* terminator */;
				char *path = (char*) malloc(length);
				sprintf(path, "%s%c%s", pConfig->directories[i], PATH_SEPARATOR, filename);
				if(nebu_FS_Test(path))
					return path;
				else
					free(path);
			}
		}
	}
	return NULL;
}

