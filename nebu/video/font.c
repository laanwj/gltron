#include "video/nebu_font.h"
#include "video/nebu_renderer_gl.h"
#include "video/nebu_video_system.h"
#include "base/nebu_surface.h"
#include "filesystem/nebu_file_io.h"
#include "filesystem/nebu_filesystem.h"

#include "base/nebu_assert.h"
#include <string.h>

#include "base/nebu_debug_memory.h"

#define SUPPORTED_IRC_COLORS 8
unsigned char irc_color_codes[8][3] = {
  { 255, 255,255 },
  { 0, 0, 0 },
  { 255, 0, 0 },
  { 255, 128, 0 },
  { 255, 255, 0 },
  { 128, 255, 0 },
  { 0, 255, 0 },
  { 0, 255, 128 }
};

/*
0     48    white       65535  65536  65535  FFFFFF
1     49    black           0      0      0  000000

2     50    red         65535      0      0  FF0000
3     51    orange      65535  32768      0  FF8000
4     52    yellow      65535  65535      0  FFFF00
5     53    lt green    32768  65535      0  80FF00
6     54    green           0  65535      0  00FF00
7     55    blue green      0  65535  32768  00FF80
8     56    cyan            0  65535  65535  00FFFF
9     57    lt blue         0  32768  65535  0080FF
:     58    blue            0      0  65535  0000FF
;     59    purple      32768      0  65535  8000FF
< 60    magenta     65535      0  65535  FF00FF
=     61    purple red  65535      0  32768  FF0080

> 62    lt gray     49152  49152  49152  C0C0C0
?     63    dk gray     16384  16384  16384  404040

@     64    -           32768      0      0  800000
A     65    |           32768  16384      0  804000
B     66    |           32768  32768      0  808000
C     67    | darker    16384  32768      0  408000
D     68    | versions      0  32768      0  008000
E     69    | of            0  32768  16384  008040
F     70    | colors        0  32768  32768  008080
G     71    | 50..61        0  16384  32768  004080
H     72    |               0      0  32768  000080
I     73    |           16384      0  32768  400080
J     74    |           32768      0  32768  800080
K     75    -           32768      0  16384  800040
*/

void getLine(char *buf, int size, file_handle file) {
  do {
    file_gets(file, buf, size);
  } while( buf[0] == '\n' || buf[0] == '#');
}

int getTextLength(const char *text, int len)
{
	int i;
	int textLength = 0;

	for(i = 0; !len || i < len; i++)
	{
		if(text[i] == 0) // end of text reached?
			break; 
		if(text[i] == 0x03) // color code?
		{
			if(text[i+1] == 0)
				break;
			i++;
		}
		else
			textLength++;
	}
	return textLength;
}


nebu_Font* nebu_Font_Load(const char *filename, int fs_tag)
{
	file_handle file;
	char buf[100];

	int i;
	int len;
	int texWidth, charWidth;

	nebu_Font *font = (nebu_Font*) malloc(sizeof(nebu_Font));

	file = file_open(filename, "r");
	/* TODO(5): check for EOF errors in the following code */

	/* nTextures, texture width, char width */
	getLine(buf, sizeof(buf), file);
	sscanf(buf, "%d %d %d ", &(font->nTextures), &texWidth, &charWidth);
	font->metrics.baseline = 0;
	font->metrics.width = (float)charWidth / (float)texWidth;
	font->metrics.height = (float)charWidth / (float)texWidth;

	/* lowest character, highest character */
	getLine(buf, sizeof(buf), file);
	{
		int lastChar;
		sscanf(buf, "%d %d ", &(font->firstChar), &lastChar);
		font->nChars = lastChar - font->firstChar;
	}
	/* font name - ignored */
	getLine(buf, sizeof(buf), file);
	len = strlen(buf) + 1;

	nebu_Video_CheckErrors("before font load");

	/* prepare space for texture IDs  */
	font->pTextures = (unsigned int*) malloc(font->nTextures * sizeof(unsigned int));
	glGenTextures(font->nTextures, (GLuint*) font->pTextures);

	/* the individual textures */
	for(i = 0; i < font->nTextures; i++) {
		char *texname;
		char *path;
		getLine(buf, sizeof(buf), file);
		len = strlen(buf) + 1;
		if(buf[len - 2] == '\n') buf[len - 2] = 0;
		texname = (char*)malloc(len);
		memcpy(texname, buf, len); 
		glBindTexture(GL_TEXTURE_2D, font->pTextures[i]);
		// TODO: load texture 
		path = nebu_FS_GetPath(fs_tag, texname);
		if(path)
		{
			nebu_Surface* p2d = nebu_Surface_LoadPNG(path);
			// TODO: add mipmapping
			// TODO: nebu_assert that p2d->w, p2d->h are valid texture dimensions
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p2d->w, p2d->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, p2d->data);
			nebu_Surface_Free(p2d);
		}

		free(path);
		free(texname);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	nebu_Video_CheckErrors("after font load");

	file_close(file);
	return font;
}

void nebu_Font_Free(nebu_Font* font)
{
	glDeleteTextures(font->nTextures, font->pTextures);
	free(font->pTextures);
	free(font);
}

void nebu_Font_GetCharsPerTexture(const nebu_Font* font, int *wx, int *wy)
{
	*wx = (int) ( 1.0 / font->metrics.width );
	*wy = (int) ( 1.0 / font->metrics.height );
}

void nebu_Font_Render(nebu_Font* font, const char *text, int len)
{
	int i;

	nebu_Video_CheckErrors("before font render");

	for(i = 0; !len || i < len; i++)
	{
		int index;
		int texture;
		int texHIndex, texVIndex;
		int wx, wy;
		float u1, u2, v1, v2;

		if(text[i] == 0)
			return;
		if(text[i] == 0x03)
		{
			i++;

			if(text[i] == 0)
				return;

			if(text[i] - '0' >= SUPPORTED_IRC_COLORS)
				continue;

			glColor4ub(
				irc_color_codes[text[i] - '0'][0],
				irc_color_codes[text[i] - '0'][1],
				irc_color_codes[text[i] - '0'][2],
				255);
			continue;
		}

		index = text[i] - font->firstChar + 1;
		if(index >= font->nChars)
		{
			// font doesn't contain this char
			nebu_assert(0);
			continue;
		}
		nebu_Font_GetCharsPerTexture(font, &wx, &wy);
		texture = index / (wx * wy);
		texHIndex = (index % (wx * wy)) % wx;
		texVIndex = wx - 1 - (index % (wx * wy)) / wx;
		u1 = texHIndex * font->metrics.width;
		u2 = (texHIndex + 1) * font->metrics.width;
		v1 = texVIndex * font->metrics.height;
		v2 = (texVIndex + 1) * font->metrics.height;

		// draw textured quad
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, font->pTextures[texture]);
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_QUADS);
		glTexCoord2f(u1, v1);
		glVertex3f(0, 0, 0);
		glTexCoord2f(u2, v1);
		glVertex3f(1, 0, 0);
		glTexCoord2f(u2, v2);
		glVertex3f(1, 1, 0);
		glTexCoord2f(u1, v2);
		glVertex3f(0, 1, 0);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		// advance vpos
		glTranslatef(1,0,0);
	}

	nebu_Video_CheckErrors("after font render");
}

void nebu_Font_RenderFormatted(nebu_Font* font, nebu_Font_Format *format, const char *text)
{
	// TODO: add word wrapping

	const char *pos = text;
	int linePos = 0;

	glPushMatrix(); // new line
	while(*pos)
	{
		if(*pos != '\n' && linePos < format->lineWidth)
		{
			nebu_Font_Render(font, pos, 1);
			linePos++;
			pos++;
		}
		else
		{
			// new line
			glPopMatrix();
			glTranslatef(0, -format->lineWidth, 0);
			glPushMatrix();

			linePos = 0;
			if(*pos == '\n')
				pos++;
		}
	}
	glPopMatrix();
}

void nebu_Font_RenderToBox(nebu_Font* font, const char *text, int len, box2 *box, int flags)
{
	float width, height, scalew, scaleh;
	int textLength;
	glPushMatrix();
	glTranslatef(box->vMin.v[0], box->vMin.v[1], 0);

	width = box->vMax.v[0] - box->vMin.v[0];
	height = box->vMax.v[1] - box->vMin.v[1];
	textLength = getTextLength(text, len);
	if(width / textLength < height)
	{
		if(flags & eFontFormatScaleFitVertically)
		{
			scaleh = height;
			scalew = width / textLength;
		}
		else
		{
			scalew = width / textLength;
			scaleh = width / textLength;
		}

	}
	else
	{
		if(flags & eFontFormatScaleFitHorizontally)
		{
			scalew = width / textLength;
			scaleh = height;
		}
		else
		{
			scalew = height;
			scaleh = height;
		}
	}

	if(flags & eFontFormatAlignCenter)
	{
		// shift according to width/scale
		glTranslatef( (width - textLength * scalew) / 2, 0, 0);
	}
	if(flags & eFontFormatAlignVCenter)
	{
		// shift according to height/scale
		glTranslatef( 0, (height - scaleh) / 2, 0);
	}

	glScalef(scalew, scaleh, 1);
	nebu_Font_Render(font, text, len);
	glPopMatrix();
}

void nebu_Font_RenderToBoxFormatted(nebu_Font* font, nebu_Font_Format *format, const char *text, box2 *box)
{
}
