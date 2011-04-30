#ifndef NEBU_FONT_H
#define NEBU_FONT_H

#include "base/nebu_vector.h"

typedef struct {
	float height; // size of a character in texture v-direction
	float width; // size of a character in texture u-direction
	float baseline; // offset to baseline in v-direction
} nebu_Font_Metrics;

typedef struct {
	int nTextures; // number of textures used
	unsigned int *pTextures; // OpenGL texture id's
	int nChars; // number of characters in this font
	int firstChar; // ascii code for the first character
	nebu_Font_Metrics metrics;
} nebu_Font;

typedef struct {
	float lineSpacing; // space between lines in glyph units
	float lineWidth; // max. line width in glyph units
	int flags; // flags for alignment, scaling
} nebu_Font_Format;

enum {
	eFontFormatAlignTop					=	0x00000000,
	eFontFormatAlignVCenter				=	0x00000001,
	eFontFormatAlignBottom				=	0x00000002,
	eFontFormatAlignLeft				=	0x00000004,
	eFontFormatAlignCenter				=   0x00000008,
	eFontFormatAlignRight				=	0x00000010,
	eFontFormatScaleFitHorizontally		=	0x00000020,
	eFontFormatScaleFitVertically		=	0x00000040,
	eFontFormatWordWrap					=	0x00000080
};

nebu_Font* nebu_Font_Load(const char *filename, int fs_tag);
void nebu_Font_Free(nebu_Font* font);

void nebu_Font_Render(nebu_Font* font, const char *text, int len);
void nebu_Font_RenderFormatted(nebu_Font* font, nebu_Font_Format *format, const char *text);
void nebu_Font_RenderToBox(nebu_Font* font, const char *text, int len, box2 *box, int flags);
void nebu_Font_RenderToBoxFormatted(nebu_Font* font, nebu_Font_Format *format, const char *text, box2 *box);

#endif

