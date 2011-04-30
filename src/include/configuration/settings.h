#ifndef SETTINGS_H
#define SETTINGS_H

/* 
 * SettingsCache - A Structure for storing lua settings during play to 
 * avoid the overhead of fetching from lua repeatedly. Values should be 
 * considered and const as they are never written back out to lua.
 */
typedef struct SettingsCache {
	int use_stencil;
	int show_scores;
	int show_ai_status;
	int ai_level;
	int show_fps;
	int show_console;
	int softwareRendering;
	int show_floor_texture;
	int line_spacing;
	int antialias_lines;
	int show_decals;
	int alpha_trails;
	int turn_cycle;
	int light_cycles;
	int lod;
	float fov;
	int stretch_textures;
	int show_skybox;
	int show_recognizer;
	int show_impact;
	int show_glow;
	int show_wall;
	int fast_finish;
	int playMusic;
	int playEffects;
	int camType;
	float znear;
	float clear_color[4];
} SettingsCache;

extern SettingsCache gSettingsCache;

int isSetting(const char *name);
float getSettingf(const char *name);
int getSettingi(const char *name);
float getVideoSettingf(const char *name);
int getVideoSettingi(const char *name);

void setSettingf(const char *name, float f);
void setSettingi(const char *name, int i);

#endif
