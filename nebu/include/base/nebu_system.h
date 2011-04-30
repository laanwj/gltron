#ifndef NEBU_SYSTEM_H
#define NEBU_SYSTEM_H

#include "SDL.h"

#include "nebu_callbacks.h"
extern Callbacks* current;

/* basically, an SDL/glut wrapper, with a few useful things added */
void nebu_Init(void); // test ok

void nebu_System_SetCallbacks(Callbacks* callbacks);
void nebu_System_SetCallback_Display( void(*display)(void) ); // test ok
void nebu_System_SetCallback_MouseMotion( void(*mouseMotion)(int, int) );
void nebu_System_SetCallback_Idle( void(*idle)(void) ); // test ok
void nebu_System_SetCallback_Key( void(*key)(int, int, int, int) );
void nebu_System_ExitLoop(int return_code); // test ok
int nebu_System_MainLoop(void); // test ok
void nebu_System_Exit(void); // test ok
void nebu_System_PostRedisplay(void); // test ok

unsigned int nebu_Time_GetElapsed();
unsigned int nebu_Time_GetElapsedSinceLastFrame();
void nebu_Time_SetCurrentFrameTime(unsigned t);
int nebu_Time_GetTimeForLastFrame();
void nebu_Time_FrameDelay(unsigned int delay);
void nebu_System_SwapBuffers(); // test ok
void nebu_System_Sleep(int ms); // test ok

/* internal functions, not for public use */
void nebu_Intern_HandleInput(SDL_Event *event);

#endif
