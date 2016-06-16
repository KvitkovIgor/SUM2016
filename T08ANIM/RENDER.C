/* FILE NAME: RENDER.C
 * PROGRAMMER: IK3
 * DATE: 15.06.2016
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
INT IK3_RndPrg;

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
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(IK3_RndMatrWorld,
    MatrMulMatr(IK3_RndMatrView, IK3_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(IK3_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(IK3_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, IK3_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(IK3_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, IK3_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(IK3_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, IK3_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(IK3_RndPrg, "Time")) != -1)
    glUniform1f(loc, IK3_Anim.Time);

  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();



  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'AS5_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       as5PRIM *Pr;
 * RETURNS: None.
 */
VOID IK3_RndPrimFree( ik3PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(ik3PRIM));
} /* End of 'Ik3_RndPrimFree' function */


