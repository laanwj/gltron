#ifndef _NEBU_DEBUG_MEMORY
#define _NEBU_DEBUG_MEMORY

/*
#ifdef _DEBUG  // Microsoft Visual C++ debug definition
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#else
#include <stdlib.h>
#endif
*/
#include <stdlib.h>

void nebu_debug_memory_CheckLeaksOnExit(void);

#endif
