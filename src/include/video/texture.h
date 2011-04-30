#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct TextureInfo {
  int count;
  char name[48];
  int type;
  int wrap_s;
  int wrap_t;
  int id;
  float anisotropy;
} TextureInfo;

enum {
	TEX_TRAIL,
	TEX_DECAL,
	TEX_SKYBOX0, TEX_SKYBOX1, TEX_SKYBOX2,
	TEX_SKYBOX3, TEX_SKYBOX4, TEX_SKYBOX5,
	TEX_IMPACT,
	/* insert all texture tokens before TEX_COUNT */
	TEX_COUNT
};

#endif

