#ifdef WIN32

#include "base/nebu_util.h"

#include <stdio.h>
#include <string.h>

#include <windows.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "base/nebu_debug_memory.h"

int isHiddenFile(WIN32_FIND_DATA *search)
{
	if(search->cFileName[0] == '.')
	{
		return 1;
	}
	else 
	{
		return search->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
	}
}

void pushFile(WIN32_FIND_DATA* pSearch, nebu_List **pL)
{
	char *filename;
	int len;

#ifdef _DEBUG
	if(strstr(pSearch->cFileName, "Makefile"))
		return;
	if(strstr(pSearch->cFileName, "CVS"))
		return;
#endif

	len = strlen(pSearch->cFileName) + 1;
	filename = malloc(len);
	memcpy(filename, pSearch->cFileName, len);
	nebu_List_AddTail(*pL, filename);
	*pL = (*pL)->next;
}

/* FIXME: This is really broken. */
nebu_List* readDirectoryContents(const char *dirname, const char *prefix) {
	WIN32_FIND_DATA search;
	HANDLE hSearch;
	nebu_List *l, *p;
	char *searchStr;
	int  prefixLen = 0;

	l = malloc(sizeof(nebu_List));
	l->next= NULL;
	p = l;

	if(prefix != NULL)
	{
		prefixLen = strlen(prefix);
	}
	searchStr = malloc(strlen(dirname) + prefixLen + 3);
	memcpy(searchStr, dirname, strlen(dirname) + 1);
	strcat(searchStr, "\\");
	if(prefix != NULL)
	{
		strcat(searchStr, prefix);
	}
	strcat(searchStr, "*");

	hSearch = FindFirstFile(searchStr, &search);
	if(hSearch == INVALID_HANDLE_VALUE)
	{
		free(l);
		free(searchStr);
		return NULL;
	}
	else if(!isHiddenFile(&search))
	{
		pushFile(&search, &p);
	}

	while( FindNextFile(hSearch, &search) )
	{
		if(!isHiddenFile(&search))
		{
			pushFile(&search, &p);
		}
	}

	if(GetLastError() != ERROR_NO_MORE_FILES)
	{
		fprintf(stderr, "[directory] Error: something bad happened\n");
	}
	
	FindClose(hSearch);
	free(searchStr);

	return l;

}

void makeDirectory(const char *name) {
	fprintf(stderr, "failed to create dir %s - feature not supported\n", name);
}

#endif
