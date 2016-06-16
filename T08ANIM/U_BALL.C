/* FILE NAME: U_BALL.C
 * PROGRAMMER: IK3
 * DATE: 15.06.2016
 * PURPOSE: Ball unit sample.
 */

#include <time.h>

#include "anim.h"

/* Ball unit representation type */
typedef struct
{
  ik3UNIT; /* Base unit fields */
  VEC Pos; /* Ball position */
  VEC Shift; /* Ball shift position */
  DBL TimerShift; /* Timer shift phase value*/
  DBL TimerSpeed; /* Timer speed value*/
  COLORREF Color; /* Color */
} ik3UNIT_BALL;

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitInit( ik3UNIT_BALL *Uni, ik3Anim *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Color = RGB(Rnd0() * 255, Rnd0() * 255, Rnd0() * 255);
} /* End of 'VG4_UnitInit' function */

/* Unit ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitResponse( ik3UNIT_BALL *Uni, ik3Anim *Ani )
{
  DBL t = Uni->TimerSpeed * clock() / 1000.0 + Uni->TimerShift;

  Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitRender( ik3UNIT_BALL *Uni, ik3Anim *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, p.X - 10, p.Y - 10, p.X + 10, p.Y + 10);
} /* End of 'VG4_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
ik3UNIT * IK3_UnitCreateBall( VOID )
{
  ik3UNIT_BALL *Uni;

  if ((Uni = (ik3UNIT_BALL *)IK3_AnimUnitCreate(sizeof(ik3UNIT_BALL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)IK3_UnitInit;
  Uni->Response = (VOID *)IK3_UnitResponse;
  Uni->Render = (VOID *)IK3_UnitRender;
  return (ik3UNIT *)Uni;
} /* End of 'VG4_UnitCreateBall' function */

/* END OF 'U_BALL.C' FILE */
