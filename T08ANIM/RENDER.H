/* FILENAME: RENDER.H
 * PROGRAMMER: IK3
 * DATE: 15.06.2016
 * PURPOSE: Animation system difinitions
*/

#ifndef __RENDER_H_
#define __RENDER_H_
#include "def.h"

/* Primitive representation type */
/* Vertex representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} ik3VERTEX;

/* Primitive representation type */
typedef struct
{
  INT VA;     /* Primitive vertex array */
  INT VBuf;   /* Point array size */
  INT IBuf;    /* Facets index array */
  INT NumOfI;/* Facets index array size */
  MATR M; /* Primitive transformation matrix */
  INT MtlNo; /* Material number */             

} ik3PRIM;
/* Ball unit representation type */

extern MATR IK3_RndMatrWorld, IK3_RndMatrView, IK3_RndMatrProj;
extern DBL IK3_RndProjSize, IK3_RndProjDist, IK3_RndFarClip;

VOID IK3_RndSetProj( VOID );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */