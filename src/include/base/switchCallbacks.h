#ifndef SWITCHCALLBACKS_H
#define SWITCHCALLBACKS_H

#include "base/nebu_callbacks.h"

extern Callbacks gameCallbacks;
extern Callbacks guiCallbacks;
extern Callbacks pauseCallbacks;
extern Callbacks configureCallbacks;
extern Callbacks promptCallbacks;
extern Callbacks creditsCallbacks;
extern Callbacks timedemoCallbacks;
extern Callbacks _32bit_warningCallbacks;

void game_Callbacks_ExitCurrent(void);
void game_Callbacks_InitCurrent(void);

void setCallback(const char *name);

#endif
