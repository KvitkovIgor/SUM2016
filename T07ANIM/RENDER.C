/* FILE NAME: RENDER.C
 * PROGRAMMER: IK3
 * DATE: 11.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */
#include "anim.h"

/* Global transformation matrices */
MATR
  IK3_RndMatrWorld, /* World (object) transformation matrix */
  IK3_RndMatrView,  /* Camera view transform */
  IK3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  IK3_RndProjDist, /* Near clip plane */
  IK3_RndFarClip ,  /* Far clip plane */
  IK3_RndProjSize ; /* Project plane size */

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID IK3_RndSetProj( VOID )

{
  DBL ratio_x = 1, ratio_y = 1;

  if (IK3_Anim.W >= IK3_Anim.H)
    ratio_x = (DBL)IK3_Anim.W / IK3_Anim.H;
  else
    ratio_y = (DBL)IK3_Anim.H / IK3_Anim.W;

  IK3_RndMatrProj = MatrFrustum(-ratio_x * IK3_RndProjSize / 2,
                                 ratio_x * IK3_RndProjSize / 2,
                                -ratio_y * IK3_RndProjSize / 2,
                                 ratio_y * IK3_RndProjSize / 2,
                                 IK3_RndProjDist, IK3_RndFarClip);
} /* End of 'IK3_RndSetProj' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *      IK3PRIM *Pr;
 * RETURNS: None.
 */


VOID IK3_RndPrimDraw( ik3PRIM *Pr )
{
  INT i;
  MATR M;
  POINT *pts;

  /* Build transform matrix  */
  M = MatrMulMatr(IK3_RndMatrWorld,
    MatrMulMatr(IK3_RndMatrView, IK3_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = PointTransform4(Pr->P[i], M);

    pts[i].x = (p.X + 1) * IK3_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * IK3_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(IK3_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(IK3_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'IK3_RndPrimDraw' function */
