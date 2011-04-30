#include "video/nebu_renderer_gl.h"
#include "base/nebu_system.h"
#include "input/nebu_input_system.h"
#include "base/nebu_callbacks.h"
#include "base/nebu_vector.h"
#include "video/nebu_font.h"

#include "video/video.h"
#include "game/resource.h"
#include "base/nebu_resource.h"

extern nebu_Font *pFont;
extern void drawMenu(Visual *d);
extern void drawGuiBackground(void);

void _32bit_warning_Display(void)
{
	nebu_Font *pInfoFont = (nebu_Font*)resource_Get(gTokenGameFont, eRT_Font);
	nebu_Font *pMenuFont = pFont;

	glClearColor(0.0, 0.0, 0.2f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// drawGuiBackground();
	// draw warning
	if(1)
	{
		nebu_Font *ppFont[5] = {
			pInfoFont,
			pInfoFont,
			pInfoFont,
			pInfoFont,
			pInfoFont
		};
		char text[5][100] = {
			"Warning - 32bit color depth",
			"not available - Please set ",
			"your desktop bitdepth to   ",
			"32bit (high color) to      ",
			"prevent visual artefacts   " };
		box2 box;
		int xoff = 20;
		int yoff = 20;
		float lineheight = 30.0f;
		int i;

		box.vMin.v[0] = (float) (0 + xoff);
		box.vMax.v[1] = (float) (gScreen->vp_h - yoff);
		box.vMax.v[0] = (float) (gScreen->vp_w - 2 * xoff);
		box.vMin.v[1] = box.vMax.v[1] - 1.2f * lineheight;

		for(i = 0; i < 5; i++)
		{
			glColor4f(1,1,1, 1.0f);
			nebu_Font_RenderToBox(ppFont[i], text[i], 0, &box, 0);
			box.vMin.v[1] -= 1.5f * lineheight;
			box.vMax.v[1] -= 1.5f * lineheight;
		}
	}
	drawMenu(gScreen);
	nebu_System_SwapBuffers();
}

void _32bit_warning_Idle(void)
{
	/*
	Sound_idle();
	scripting_RunGC();
	Video_Idle();
	Input_Idle();
	nebu_Time_FrameDelay(50);
	*/
	nebu_System_PostRedisplay(); /* animate menu */
}

void _32bit_warning_Key(int state, int key, int x, int y)
{
	if(state != NEBU_INPUT_KEYSTATE_DOWN)
		return;
    switch (key)
	{
	case SYSTEM_KEY_ESCAPE:
		// exit callback to gui
		break;
	case SYSTEM_KEY_DOWN:
		// next menu
		break;
	case SYSTEM_KEY_UP:
		// prev menu
		break;
	}
}

void _32bit_warning_Init(void)
{
}

void _32bit_warning_Exit(void)
{
}

Callbacks _32bit_warningCallbacks = {
	_32bit_warning_Display, // display
	_32bit_warning_Idle, // idle
	_32bit_warning_Key, // keyboard
	_32bit_warning_Init, // init
	_32bit_warning_Exit, // exit
	NULL, // mouse
	NULL, // mouseMotion
	"32bit_warning"
};

