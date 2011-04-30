#include "base/switchCallbacks.h"
#include "base/nebu_callbacks.h"
#include "base/nebu_system.h"

#include "base/nebu_debug_memory.h"

#include <string.h>
#include <stdio.h>
#include "base/nebu_assert.h"

Callbacks *last_callback = NULL;
Callbacks *current_callback = NULL;
  
void game_Callbacks_ExitCurrent(void) {
	/* called when the window is recreated */
  if(current_callback && current_callback->exit)
    (current_callback->exit)(); /* give them the chance to quit */
}

void game_Callbacks_InitCurrent(void)
{
	/* called when the window is recreated */
	nebu_System_SetCallbacks(current_callback);
	// calls init
}

#define N_CALLBACKS 7
Callbacks *callbackList[N_CALLBACKS] = {
	&gameCallbacks, &guiCallbacks, &pauseCallbacks, &configureCallbacks,
	&promptCallbacks, &creditsCallbacks, &timedemoCallbacks
};

void setCallback(const char *name) {
	int i;

	for(i = 0; i < N_CALLBACKS; i++) {
		if(strcmp( callbackList[i]->name, name ) == 0)
			break;
	}
	if(i == N_CALLBACKS) {
		fprintf(stderr, "fatal: no callback named '%s' found\n", name);
		nebu_assert(0); exit(1); // OK: programmer error, critical
	}

	last_callback = current_callback;
	current_callback = callbackList[i];

	nebu_System_SetCallbacks(callbackList[i]);
}
