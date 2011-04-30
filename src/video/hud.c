#include "video/hud.h"

static void drawFrames(void);
static void drawFPS(void);
static void drawAIStatus(void);
static void drawHUDFrame(void);
static void drawSpeedometer(void);
static void drawScores(void);


void drawHud(void) {
	drawFrames();
	drawSpeedometer();
	drawFPS();
	drawAIStatus();
}

void drawFrame(void) {
}

void drawFPS(void) {
}

void drawAIStatus(void) {
}

void drawHUDFrame(void) {
}

void drawSpeedometer(void) {
}

void drawScores(void) {
}
