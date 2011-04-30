#ifndef NEBU_VIDEO_SYSTEM_H
#define NEBU_VIDEO_SYSTEM_H

#define SYSTEM_RGB 1
#define SYSTEM_DOUBLE 2
#define SYSTEM_DEPTH 4
#define SYSTEM_FULLSCREEN 8
#define SYSTEM_32_BIT 16
#define SYSTEM_STENCIL 32
#define SYSTEM_ALPHA 64
#define SYSTEM_RGBA (SYSTEM_RGB | SYSTEM_ALPHA)

extern void SystemReshapeFunc(void(*reshape)(int, int));
extern void SystemSetGamma(float r, float g, float b);

void nebu_Video_Init(void); // test ok

void nebu_Video_SetWindowMode(int x, int y, int w, int h); // test ok
void nebu_Video_SetDisplayMode(int flags); // test ok
void nebu_Video_GetDisplayDepth(int *r, int *g, int *b, int *a);
int nebu_Video_Create(char *name); // test ok
void nebu_Video_Destroy(int id); // test ok
void nebu_Video_GetDimension(int *x, int *y);

void nebu_Video_WarpPointer(int x, int y);
void nebu_Video_CheckErrors(const char *where);

#endif
