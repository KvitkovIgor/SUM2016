/* FILE NAME: VEC.H
 * PROGRAMMER: IK3
 * DATE: 08.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */
#include "anim.h"
/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       IK3UNIT *Uni;
 *   - animation context:
 *       IK3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitInit( ik3UNIT *Uni, ik3Anim *Ani )
{
} /* End of 'IK3_UnitInit' function */


/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       IK3UNIT *Uni;
 *   - animation context:
 *       IK3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitClose( ik3UNIT *Uni, ik3Anim *Ani )
{
} /* End of 'IK3_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       IK3UNIT *Uni;
 *   - animation context:
 *       IK3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitResponse( ik3UNIT *Uni, ik3Anim *Ani )
{
} /* End of 'IK3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       IK3UNIT *Uni;
 *   - animation context:
 *       IK3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitRender( ik3UNIT *Uni, ik3Anim *Ani )
{
} /* End of 'IK3_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (IK3UNIT *) pointer to created units */
ik3UNIT * IK3_AnimUnitCreate( INT Size )
{
  ik3UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(ik3UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = IK3_UnitInit;
  Uni->Close = IK3_UnitClose;
  Uni->Response = IK3_UnitResponse;
  Uni->Render = IK3_UnitRender;
  return Uni;
} /* End of 'IK3_AnimUnitCreate' function */


