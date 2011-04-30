#ifndef GRAPHICS_LIGHTS_H
#define GRAPHICS_LIGHTS_H

typedef enum eLightEnvironment {
	eCyclesAmbient = 0,
	eCyclesWorld,
	eRecognizer,
	eWorld
} eLightEnvironment;

void setupLights(eLightEnvironment eType);

#endif
