#include "base/nebu_debug_memory.h"
#include "base/nebu_assert.h"

#include <stdio.h>
#include <assert.h>

// review: 64bit ok
static int iConfig = NEBU_ASSERT_DEFAULT_CONFIG;

void nebu_debug_memory_CheckLeaksOnExit(void)
{
#ifdef _DEBUG
	// _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
}

void nebu_assert_config(int flags)
{
	iConfig = flags;
}

void nebu_assert_int(int value)
{
	if(!value)
	{
		if(iConfig & NEBU_ASSERT_LIBC)
		{
			assert(value);
			// _DbgBreak();
		}
		if(iConfig & NEBU_ASSERT_PRINT_STDERR)
		{
			fprintf(stderr, "[NEBU_ASSERT] assert triggered\n");
		}
	}
}
