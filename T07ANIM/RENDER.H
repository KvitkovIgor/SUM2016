/* FILENAME: RENDER.H
 * PROGRAMMER: IK3
 * DATE: 13.06.2016
 * PURPOSE: Animation system difinitions
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "anim.h"

/* Primitive representation type */
typedef struct
{
  VEC *P;          /* Primitive points array */
  INT NumOfP;      /* Point array size */
  INT (*Edges)[2]; /* Edges array - array of point pairs */
  INT NumOfE;      /* Edges array size */
} ik3PRIM;
/* Vertex representation type  */
typedef struct                  
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} ik3VERTEX;

extern MATR IK3_RndMatrWorld, IK3_RndMatrView, IK3_RndMatrProj;
extern DBL IK3_RndProjSize, IK3_RndProjDist, IK3_RndFarClip;



VOID IK3_RndSetProj( VOID );
IK3_RndPrimLoad( ik3PRIM *Pr, CHAR *FileName );
VOID IK3_RndPrimDraw( ik3PRIM *Pr );
VOID IK3_RndPrimFree( ik3PRIM *Pr );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */