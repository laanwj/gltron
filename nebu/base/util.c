/* small utility functions */

#include "base/nebu_random.h"
#include "base/nebu_util.h"

#include "base/nebu_assert.h"

#include "base/nebu_debug_memory.h"

// review: 64bit ok

void nebu_RandomPermutation( int N, int *nodes )
{
  int i;

  for(i = 0; i < N - 1; i++) {
    int s, t;
    int tmp;
    t = N - 1 - i;
    // s = (int) ((float)( t + 1 ) * nebu_rand() / (RAND_MAX + 1.0f));
    s = nebu_rand() % (t + 1);
    tmp = nodes[t];
    nodes[t] = nodes[s];
    nodes[s] = tmp;
  }
}

void nebu_Clamp( float *f, float min, float max )
{
  if(*f < min) *f = min;
  else if(*f > max) *f = max;
}

nebu_List* nebu_List_Create(void)
{
	nebu_List *p = (nebu_List*) malloc(sizeof(nebu_List));
	p->data = NULL;
	p->next = NULL;
	return p;
}


void nebu_List_Free(nebu_List *l)
{
	nebu_List *pNext;
	nebu_List *p;
	for(p = l; p;)
	{
		pNext = p->next;
		free(p);
		p = pNext;
	}
}

int nebu_List_IsEmpty(const nebu_List *l)
{
	return (l->next == NULL);
}

void nebu_List_AddTail(nebu_List *l, void* data)
{
	nebu_List *p;

	for(p = l; p->next != NULL; p = p->next);
	p->next = (nebu_List*) malloc(sizeof(nebu_List));
	p->next->next = NULL;
	p->data = data;
}

// debug code to track down memory leaks
void nebu_List_AddTail2(nebu_List *l, void* data)
{
	nebu_List *p;

	for(p = l; p->next != NULL; p = p->next);
	p->next = (nebu_List*) malloc(sizeof(nebu_List));
	p->next->next = NULL;
	p->data = data;
}

void nebu_List_RemoveAt(nebu_List *pItem, nebu_List *pPrevItem)
{
	if(!pItem->next)
	{
		nebu_assert(0);
		return;
		// don't free the empty list tail
	}
	if(pPrevItem)
	{
		pPrevItem->next = pItem->next;
		free(pItem);
	}
	else
	{
		// don't free list head, free the next item instead
		nebu_List *pNext = pItem->next;
		*pItem = *pNext;
		free(pNext);
	}
}
