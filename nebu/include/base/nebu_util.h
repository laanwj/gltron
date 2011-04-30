#ifndef NEBU_UTIL_H
#define NEBU_UTIL_H

/* small utility datastructures & functions */

typedef struct nebu_List nebu_List;
struct nebu_List {
  void *data;
  nebu_List* next;
};

void nebu_RandomPermutation( int N, int *nodes );
void nebu_Clamp( float *f, float min, float max );

void nebu_List_AddTail(nebu_List *l, void* data);
void nebu_List_AddTail2(nebu_List *l, void* data);
void nebu_List_RemoveAt(nebu_List *pItem, nebu_List *pPrevItem);
nebu_List* nebu_List_Create();
void nebu_List_Free(nebu_List* l);
int nebu_List_IsEmpty(const nebu_List *l);
#endif
