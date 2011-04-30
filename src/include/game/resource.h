#ifndef RESOURCE_H
#define RESOURCE_H

#include "video/nebu_2d.h"

typedef enum EResourceType
{
	eRT_Surface,
	eRT_2d,
	eRT_Texture,
	eRT_Font,
	eRT_GLtronTriMesh,
	eRT_GLtronQuadMesh
} EResourceType;

void resource_Init(void);
void resource_Shutdown(void);


#endif /* RESOURCE_H */
