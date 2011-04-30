#include "video/trail_geometry.h"
#include "video/nebu_video_system.h"
#include "video/nebu_renderer_gl.h"
#include "game/game.h"
#include "configuration/settings.h"

void trailStatesNormal(Player *pPlayer, int texture) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	// glDisable(GL_TEXTURE_2D);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	{ 
		float black[] = { 0, 0, 0, 1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}

	if(gSettingsCache.alpha_trails) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void trailStatesShadowed(void) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	// glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void trailStatesRestore(void) {
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	if(!getSettingi("wireframe"))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void trailRender(TrailMesh *pMesh) {
	if(pMesh->iUsed == 0)
		return;
	
	nebu_Video_CheckErrors("before trail va setup");

	glVertexPointer(3, GL_FLOAT, 0, pMesh->pVertices);
	glNormalPointer(GL_FLOAT, 0, pMesh->pNormals);
	glTexCoordPointer(2, GL_FLOAT, 0, pMesh->pTexCoords);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, pMesh->pColors);

	nebu_Video_CheckErrors("after trail va setup");

	glDrawElements(GL_TRIANGLES, pMesh->iUsed, GL_UNSIGNED_SHORT, pMesh->pIndices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	nebu_Video_CheckErrors("trail");
}
