#include "filesystem/path.h"
#include "game/gltron.h"
#include "game/timesystem.h"
#include "base/nebu_callbacks.h"
#include "game/game.h"
#include "game/camera.h"
#include "game/engine.h"
#include "input/input.h"
#include "input/nebu_input_system.h"
#include "video/video.h"
#include "audio/audio.h"
#include "video/nebu_video_system.h"
#include "scripting/nebu_scripting.h"
#include "audio/sound_glue.h"
#include "configuration/settings.h"
#include "configuration/configuration.h"

#include "base/nebu_debug_memory.h"

#include "base/nebu_assert.h"

void GameMode_Idle(void) {
	Sound_idle();
	Time_Idle();
	if(game2->time.dt == 0)
		return;
	Game_Idle();
	Video_Idle();
	Input_Idle();
	Scripting_Idle();
	nebu_Time_FrameDelay(5);
	nebu_System_PostRedisplay();
}

void enterGame(void) { /* called when game mode is entered */
	updateSettingsCache();

	nebu_Input_HidePointer();
	nebu_Input_Mouse_WarpToOrigin();
	game2->time.offset = nebu_Time_GetElapsed() - game2->time.current;
	Audio_EnableEngine();
 
	// disable booster & wallbuster
	{
		int i;
		for(i = 0; i < game->players; i++) {
			game->player[i].data.boost_enabled = 0;
			game->player[i].data.wall_buster_enabled = 0;
		}
	}
	/* fprintf(stderr, "init game\n"); */
}

void exitGame(void) {
  Audio_DisableEngine();
  /* fprintf(stderr, "exit game\n"); */
}

void gameMouse(int buttons, int state, int x, int y) {
	if(state == SYSTEM_MOUSEPRESSED) {
		if(buttons == SYSTEM_MOUSEBUTTON_LEFT) gInput.mouse1 = 1;
		if(buttons == SYSTEM_MOUSEBUTTON_RIGHT) gInput.mouse2 = 1;
	} else if(state == SYSTEM_MOUSERELEASED) {
		if(buttons == SYSTEM_MOUSEBUTTON_LEFT) gInput.mouse1 = 0;
		if(buttons == SYSTEM_MOUSEBUTTON_RIGHT) gInput.mouse2 = 0;
	}

  /*
  if(getSettingi("camType") == CAM_TYPE_MOUSE) 
    if(state == SYSTEM_MOUSEPRESSED) {
      if(buttons == SYSTEM_MOUSEBUTTON_LEFT) {
	cam_r -= CAM_DR;
	if(cam_r < CAM_R_MIN) cam_r = CAM_R_MIN;
      } else if(buttons == SYSTEM_MOUSEBUTTON_RIGHT) {
	cam_r += CAM_DR;
	if(cam_r > CAM_R_MAX) cam_r = CAM_R_MAX;
      }
    }
  */
  /* fprintf(stderr, "new cam_r: %.2f\n", cam_r); */
}

Callbacks gameCallbacks = { 
  displayGame, GameMode_Idle, keyGame, enterGame, exitGame, gameMouse, NULL, "game"
};
