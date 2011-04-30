#include "input/nebu_input_system.h"
#include "base/nebu_system.h"
#include "video/nebu_scene.h"

extern nebu_Scene *pScene;

void checkKeyboard(int dt)
{
	float d = 0.15f;
	float dist;

#define NUMKEYS 6
	char keys[NUMKEYS] = { 'w', 's', 'a', 'd', 'c', ' ' };

	int i;

	for(i = 0; i < NUMKEYS; i++)
	{
		if(nebu_Input_GetKeyState(keys[i]) != NEBU_INPUT_KEYSTATE_DOWN)
			continue;

		dist = d * dt;
		switch(keys[i])
		{
		case 'w': nebu_Camera_Slide(pScene->pCamera, 0, 0, dist); break;
		case 's': nebu_Camera_Slide(pScene->pCamera, 0, 0, -dist); break;
		case 'a': nebu_Camera_Slide(pScene->pCamera, -dist, 0, 0); break;
		case 'd': nebu_Camera_Slide(pScene->pCamera, dist, 0, 0); break;
		case 'c': nebu_Camera_Slide(pScene->pCamera, 0, -dist, 0); break;
		case ' ': nebu_Camera_Slide(pScene->pCamera, 0, dist, 0); break;
		}
		nebu_System_PostRedisplay();
	}
}