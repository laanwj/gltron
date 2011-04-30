#include "video/video.h"
#include "video/graphics_lights.h"
#include "video/nebu_renderer_gl.h"

void setupLights(eLightEnvironment eType)
{
	float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// float gray22[] = { 0.22f, 0.22f, 0.22f, 1.0f };
	float gray88[] = { 0.88f, 0.88f, 0.88f, 1.0f };
	float gray66[] = { 0.66f, 0.66f, 0.66f, 1.0f };
	float gray50[] = { 0.50f, 0.50f, 0.50f, 1.0f };
	float gray10[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	float posWorld0[] = { 1, .8f, 0, 0 };
	float posWorld1[] = { -1, -.8f, 0, 0 };

	float posCycles0[] = { 0, 0, 1, 0 };
	float posCycles1[] = { .5, .5, 1, 0 };
	
	int i;

	if(gIsRenderingReflection) {
		posCycles0[2] *= -1;
		vec3_Scale( (vec3*)posCycles1, (vec3*)posCycles1, -1);
	}

	// turn global ambient lighting off
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);

	switch(eType) {
	case eWorld:
	case eCyclesWorld:
		glMatrixMode(GL_MODELVIEW);
		// glPushMatrix();
		// glLoadIdentity();
		
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, posWorld0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, gray10);
		// glLightfv(GL_LIGHT0, GL_AMBIENT, black);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

		if(eType == eCyclesWorld)
		{
			glLightfv(GL_LIGHT0, GL_POSITION, posCycles1);
			glDisable(GL_LIGHT1);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, gray88);
		}
		else
		{
			glEnable(GL_LIGHT1);
			glLightfv(GL_LIGHT1, GL_POSITION, posWorld1);
			glLightfv(GL_LIGHT1, GL_AMBIENT, black);
			glLightfv(GL_LIGHT1, GL_SPECULAR, gray66);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, gray66);
			
			for(i = 2; i < 8; i++)
				glDisable(GL_LIGHT0 + i);
		}
		// glPopMatrix();

		break;
	case eCyclesAmbient:
	case eRecognizer:
		glEnable(GL_LIGHT0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		glLightfv(GL_LIGHT0, GL_POSITION, posCycles0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, gray10);
		if(eType == eRecognizer)
		{
			glLightfv(GL_LIGHT0, GL_SPECULAR, white);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		}
		else
		{
			glLightfv(GL_LIGHT0, GL_SPECULAR, gray50);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, gray50);
		}

		for(i = 1; i < 8; i++)
			glDisable(GL_LIGHT0 + i);
		
		glPopMatrix();
		break;
	}
}		
