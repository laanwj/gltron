#ifndef NEBU_QUAD_H
#define NEBU_QUAD_H

/* 
   this file holds datastructures for quads
   they can be stored in Quad's and QuadBuffers for later processing
   and rendering
*/

/* fixme: this is all very obsolete, use vertex buffers instead */

typedef struct Vertex {
  float v[3];
  float color[4];
  float uv[2];
} Vertex;

typedef struct Quad {
  Vertex vertex[4]; 
  int type;
  int texture_id;
} Quad;

typedef struct Tris {
  Vertex vertex[3]; 
  int type;
  int texture_id;
} Tris;

typedef struct QuadBuffer {
  Quad *quads;
  int size;
  int current;
} QuadBuffer;

typedef struct QuadBufInfo {
  QuadBuffer *qbuf;
  int *qIndex;
  int size;
  int *qFlags;
} QuadBufInfo;

typedef struct TrisBuffer {
  Tris *quads;
  int size;
  int current;
} TrisBuffer;

typedef struct TrisBufInfo {
  TrisBuffer *qbuf;
  int *qIndex;
  int size;
  int *qFlags;
} TrisBufInfo;

enum { 
  QUAD_COLOR = 1,
  QUAD_TEXTURE = 2,
  QUAD_TEX_MODULATE = 4,
  QUAD_TEX_DECAL = 8,
  QUAD_ALPHA_TEST = 16
};

/* quad.c */

extern void q_setColor4fv( Quad *q, int index, float color[4] );
extern void q_setColor3f( Quad *q, int index, float r, float g, float b);
extern void q_setColor4f( Quad *q, int index, float r, float g, float b, float a);
extern void q_setTexCoord2f( Quad *q, int index, float u, float v );
extern void q_setVertex3f( Quad *q, int index, float x, float y, float z);
extern void q_setType(Quad *q, int type);
extern QuadBuffer* createQuadBuffer(int size);
extern void freeQuadBuffer(QuadBuffer* q );
extern Quad* getNextQuad(QuadBuffer* q);
extern int getCurrentQuad(QuadBuffer* q);
extern int getQuadBufferSize(QuadBuffer *q);
extern void renderQuadAt(QuadBuffer *q, int *index, int i);

/* quadbuf.c */
extern int* getSortedQuads(QuadBuffer *q, float eye[3]);
extern Quad* getQuadAt(QuadBuffer *q, int index);

/* extern renderer_ lib must provide this */
extern void renderQuad(Quad* q);
#endif

