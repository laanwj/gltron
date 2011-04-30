#include "video/video.h"
#include "game/game.h"
#include "video/nebu_renderer_gl.h"
#include "game/resource.h"
#include "base/nebu_resource.h"
#include "video/graphics_lights.h"

void drawWalls(void) {
	int i;

	setupLights(eCyclesWorld);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(i = 0; i < gWorld->arena_shader.passes; i++)
	{
		video_Shader_Setup(& gWorld->arena_shader, i);
		video_Shader_Geometry(gWorld->arena, TRI_MESH, i);
		video_Shader_Cleanup(& gWorld->arena_shader, i);
	}
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

