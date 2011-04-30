#ifndef NEBU_RESOURCE_H
#define NEBU_RESOURCE_H

void resource_RegisterHandler(int type, void* (*get)(char*,void*), void (*release)(void *));
void resource_UnregisterHandler(int type);

void resource_Release(int token);
void resource_ReleaseAll(void);
void resource_ReleaseType(int type);
void resource_FreeAll(void);
void resource_Free(int token);
void* resource_Get(int token, int type);
int resource_GetToken(char *filename, int type);
int resource_GetTokenMeta(char *filename, int type, void *metadata, int metasize);

#endif