#include "video/nebu_video_system.h"
#include "video/nebu_camera.h"
#include "video/nebu_scene.h"
#include "video/nebu_renderer_gl.h"
#include "base/nebu_system.h"

extern nebu_Scene* pScene;
extern void checkKeyboard(int dt);

void idle() {
	int dt;
	nebu_System_Sleep(10);
	dt = nebu_Time_GetElapsedSinceLastFrame();
	if(dt < 10)
		return;
	nebu_Time_SetCurrentFrameTime( nebu_Time_GetElapsed() );
	checkKeyboard(dt);
}

void display() {
	glClearColor(0, .3f, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	nebu_Scene_Draw(pScene);
	nebu_System_SwapBuffers();
}