#include "game/gltron.h"
#include "game/game.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "video/video.h"
#include "video/graphics_utility.h"
#include "video/nebu_video_system.h"
#include "video/nebu_renderer_gl.h"
#include "base/nebu_callbacks.h"
#include "scripting/nebu_scripting.h"
#include "input/nebu_input_system.h"
#include "scripting/scripting.h"

static int coffset;

char *credits[] = {
  "",
  "   GLtron is written by " "\x03" "4" "Andreas Umbach",
  "",
  " Additional",
  " Programming: Darrell Walisser  Nicolas Deniaud",
  "              Todd Kirby  Andy Howe  Jon Atkins",
  " Modeling:    Peter \"VORIX\" Blight           ",
  "              Tracy Brown                      ",
  " Texturing:	 Nicolas Zimmermann               ",
  "              Tyler Esselstrom       Allen Bond",
  " Music:       Peter Hajba",
  " Sound:       Damon Law",
  "",
  "",
  "Additional Thanks to:",
  "Xavier Bouchoux     Mike Field      Steve Baker",
  "Jean-Bruno Richard             Andrey Zahkhatov",
  "Bjonar Henden   Shaul Kedem    Jonas Gustavsson",
  "Mattias Engdegard     Ray Kelm     Thomas Flynn",
  "Martin Fierz    Joseph Valenzuela   Ryan Gordon",
  "Sam Lantinga  Patrick McCarthy  Charles Babbage",
  // "Thanks to my sponsors:",
  // "  3dfx:              Voodoo5 5500 graphics card",
  // "  Right Hemisphere:  3D exploration software",
  NULL
};

void mouseCredits (int buttons, int state, int x, int y)
{
	if ( state == SYSTEM_MOUSEPRESSED ) {
		nebu_System_Exit();
		nebu_System_ExitLoop(eSRC_Quit);
	}
}

void keyCredits(int state, int k, int x, int y)
{
	if(state == NEBU_INPUT_KEYSTATE_UP)
		return;
	nebu_System_Exit();
	nebu_System_ExitLoop(eSRC_Quit);
}

void idleCredits(void) {
	scripting_RunGC();
	nebu_Time_FrameDelay(50);
	nebu_System_PostRedisplay();
}

void drawCredits(void) {
  int time;
  float x = 10.0f;
  float y;
  float h = gScreen->vp_h / 36.0f;
  int i;
  float colors[][3] = { { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } };
  time = nebu_Time_GetElapsed() - coffset;

  glClearColor(.0, .0, .0, .0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rasonly(gScreen);
  for(i = 0; i < time / 250; i++) {
    glColor4f(
		colors[i % 2][0],
		colors[i % 2][1],
		colors[i % 2][2],
		1);
    if(credits[i] == NULL) 
      break;
    y = gScreen->vp_h - 3.0f * h * (i + 1) / 2;
    drawText((nebu_Font*)resource_Get(gTokenGameFont, eRT_Font), x, y, h, credits[i]);
  }
}
void displayCredits(void) {
  drawCredits();
  nebu_System_SwapBuffers();
}

void initCredits(void) {
  coffset = nebu_Time_GetElapsed();
}

Callbacks creditsCallbacks = { 
  displayCredits, idleCredits, keyCredits, initCredits, 
  NULL /* exit */, mouseCredits, NULL /* mouse motion */, "credits"
};
