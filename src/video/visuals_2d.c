#include "video/video.h"
#include "game/game.h"
#include "configuration/settings.h"
#include "game/game_level.h"

#include "base/nebu_math.h"
#include "video/nebu_renderer_gl.h"

#include "base/nebu_debug_memory.h"
/* draw a 2d map */

void draw2D( nebu_Rect *pRect )
{
	// TODO: modify for nebu_rect
	// get rid of the viewport requirement
	// assume rasonly is set, push/pop matrices
	int i;

	float grid_width = box2_Width(& game2->level->boundingBox);
	float grid_height = box2_Height(& game2->level->boundingBox);

	glPushMatrix();
	glTranslatef( (float)pRect->x, (float)pRect->y, 0);
	glScalef(pRect->width / grid_width, pRect->height / grid_height, 1);
	glTranslatef(
		-game2->level->boundingBox.vMin.v[0],
		-game2->level->boundingBox.vMin.v[1],
		0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1, 1, 1, 1.0f);
	{
		int nVertices = game2->level->nBoundaries * 2;
		float *pVertices = (float*) malloc(3 * nVertices * sizeof(float));
		for(i = 0; i < game2->level->nBoundaries; i++) {
			vec2 v;
			segment2 *s = game2->level->boundaries + i;
			vec2_Add(&v, & s->vStart, & s->vDirection);
			pVertices[6 * i + 0] = s->vStart.v[0];
			pVertices[6 * i + 1] = s->vStart.v[1];
			pVertices[6 * i + 2] = 0;
			pVertices[6 * i + 3] = v.v[0];
			pVertices[6 * i + 4] = v.v[1];
			pVertices[6 * i + 5] = 0;
		}
		glVertexPointer(3, GL_FLOAT, 0, pVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_LINES, 0, nVertices);
		glDisableClientState(GL_VERTEX_ARRAY);

		free(pVertices);
	}

	// find out number of trails to draw into the 2d map
	// create vertex & color arrays
	// fill in data
	// draw everything in one go

	{
		int nVertices = 0;

		for(i = 0; i < game->players; i++) {
			Player *p = &game->player[i];
			if (p->data.trail_height <= 0) {
				continue;
			}
			// TODO: get vertex count
			nVertices += 2 * p->data.nTrails;
		}
	}

	for(i = 0; i < game->players; i++) {
		Player *p = &game->player[i];
		int i;
		float x, y;

		if (p->data.trail_height <= 0) {
			continue;
		}

		if (p->data.trail_height < TRAIL_HEIGHT) {
			/* 
				if player crashed but the trail hasn't disappeared yet, fade
				the trail on the 2d map as it disappears.
			*/
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			glColor4f(p->profile.pColorAlpha[0], p->profile.pColorAlpha[1], p->profile.pColorAlpha[2],
					p->data.trail_height / TRAIL_HEIGHT);
		} else {
			glBlendFunc(GL_ONE, GL_ONE);
			glColor4f(p->profile.pColorAlpha[0],
				p->profile.pColorAlpha[1],
				p->profile.pColorAlpha[2],
				p->profile.pColorAlpha[3]);
		}

		getPositionFromData(&x, &y, &p->data);
		
		glPointSize(2);
		glBegin(GL_POINTS);
		glVertex2f( x, y );
		glEnd();

		glBegin(GL_LINES);
		for(i = 0; i < p->data.nTrails; i++)
		{
			segment2* trail = p->data.trails + i;
			glVertex2f(trail->vStart.v[0], 
				trail->vStart.v[1]
				);
			glVertex2f(trail->vStart.v[0] + trail->vDirection.v[0], 
				trail->vStart.v[1] + trail->vDirection.v[1]
				);
		}
		// I have no idea what the following was necessary for
		// TODO: figure it out and rewrite or remove
		/*
		if(trail != p->data.trails)
		{
			trail--;
			glVertex2f(trail->vStart.v[0] + trail->vDirection.v[0], 
				trail->vStart.v[1] + trail->vDirection.v[1]
				);
			glVertex2f( floorf(x), floorf(y));
		}
		else
		{
			glVertex2f(trail->vStart.v[0], 
				trail->vStart.v[1]
				);
			glVertex2f( floorf(x), floorf(y));
		}
		*/
			
	#if 0
		// draw AI debug lines
		glColor4f(1,1,1, 1.0f);
		glVertex2f(p->ai.front.vStart.v[0],
			p->ai.front.vStart.v[1]);
		glVertex2f(p->ai.front.vStart.v[0] + p->ai.front.vDirection.v[0],
			p->ai.front.vStart.v[1] + p->ai.front.vDirection.v[1]);
		glColor4f(0,1,0, 1.0f);
		glVertex2f(p->ai.left.vStart.v[0],
			p->ai.left.vStart.v[1]);
		glVertex2f(p->ai.left.vStart.v[0] + p->ai.left.vDirection.v[0],
			p->ai.left.vStart.v[1] + p->ai.left.vDirection.v[1]);
		glColor4f(0,0,1, 1.0f);
		glVertex2f(p->ai.right.vStart.v[0],
			p->ai.right.vStart.v[1]);
		glVertex2f(p->ai.right.vStart.v[0] + p->ai.right.vDirection.v[0],
			p->ai.right.vStart.v[1] + p->ai.right.vDirection.v[1]);
		glColor4f(0,1,1, 1.0f);
		glVertex2f(p->ai.backleft.vStart.v[0],
			p->ai.backleft.vStart.v[1]);
		glVertex2f(p->ai.backleft.vStart.v[0] + 
			p->ai.backleft.vDirection.v[0],
			p->ai.backleft.vStart.v[1] + 
			p->ai.backleft.vDirection.v[1]);
	#endif
		glEnd();
	}
	glDisable(GL_BLEND);

	glPopMatrix();
}
	
