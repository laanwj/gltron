#ifndef GRAPHICS_FX_H
#define GRAPHICS_FX_H

#include "video/video.h" // PlayerVisual
#include "game/game_data.h" // Player
#include "game/camera.h" // Camera
#include "video/nebu_video_types.h" // Visual

void drawImpact(int player);
void drawGlow(PlayerVisual *pV, Player *pTarget, float dim);

#endif
