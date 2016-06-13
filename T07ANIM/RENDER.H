/* FILE NAME: RENDER.H
 * PROGRAMMER: IK3
 * DATE: 11.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */
/* Primitive representation type */
#ifndef __RENDER_H_
#define __RENDER_H_

#include "anim.h"

typedef struct
{
  VEC *P;          /* Primitive points array */
  INT NumOfP;      /* Point array size */
  INT (*Edges)[2]; /* Edges array - array of point pairs */
  INT NumOfE;      /* Edges array size */
} ik3PRIM;

VEC CubeP[] =
{
  {-1, -1,  1},
  { 1, -1,  1},
  { 1, -1, -1},
  {-1, -1, -1},
  {-1,  1,  1},
  { 1,  1,  1},
  { 1,  1, -1},
  {-1,  1, -1}
};

INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};
ik3PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};
MATR
  IK3_RndMatrWorld,
  IK3_RndMatrView,  
  IK3_RndMatrProj;  

DBL
  IK3_RndProjDist, 
  IK3_RndFarClip,  
  IK3_RndProjSize; 

#endif
