#ifndef NEBU_TEXTURE2D_H
#define NEBU_TEXTURE2D_H

typedef struct {
	unsigned int id;
} nebu_Texture2D;

typedef struct {
	int min_filter;
	int mag_filter;
	int wrap_s;
	int wrap_t;
	int format;
	float anisotropy;
} nebu_Texture2D_meta;

nebu_Texture2D* nebu_Texture2D_Load(const char *path, const nebu_Texture2D_meta* meta);
void nebu_Texture2D_Free(nebu_Texture2D* pTexture);
#endif
