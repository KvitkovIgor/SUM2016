/* FILE NAME: U_CUBE.C
 * PROGRAMMER: IK3
 * DATE: 15.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */
#include <time.h>
#include "render.h"
#include "anim.h"
#include "units.h"

typedef struct
{
  ik3UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  ik3OBJ Pr;
  DBL TimerSpeed;  /* Timer speed value*/
} ik3UNIT_CUBE;

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ik3UNIT_CUBE *Uni;
 *   - animation context:
 *       ik3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitInit( ik3UNIT_CUBE *Uni, ik3Anim *Ani )
{
  IK3_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.03F, 0.03F, 0.03F)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                                         VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3)))));

  IK3_RndObjLoad(&Uni->Pr, "Kerrigan_infested\\Kerrigan_infested.g3d");
} /* End of 'IK3_UnitInit' function */

/* Unit cube deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ik3UNIT_CUBE *Uni;
 *   - animation context:
 *       ik3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitClose( ik3UNIT_CUBE *Uni, ik3Anim *Ani )
{
   IK3_RndObjFree(&Uni->Pr);
} /* End of 'IK3_UnitClose' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ik3UNIT_CUBE *Uni;
 *   - animation context:
 *       ik3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitResponse( ik3UNIT_CUBE *Uni, ik3Anim *Ani )
{
} /* End of 'IK3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       IK3UNIT_CUBE *Uni;
 *   - animation context:
 *       IK3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitRender( ik3UNIT_CUBE *Uni, ik3Anim *Ani )
{
  /* IK3_RndMatrWorld = MatrScale(VecSet(0.3, 0.3, 0.3)); */
  IK3_RndObjDraw(&Uni->Pr);
} /* End of 'IK3_UnitRender' function */

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (IK3UNIT *) pointer to created unit.
 */
ik3UNIT * IK3_UnitCreateCube( VOID )
{
  ik3UNIT_CUBE *Uni;

  if ((Uni = (ik3UNIT_CUBE *)IK3_AnimUnitCreate(sizeof(ik3UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)IK3_UnitInit;
  Uni->Close = (VOID *)IK3_UnitClose;
  Uni->Response = (VOID *)IK3_UnitResponse;
  Uni->Render = (VOID *)IK3_UnitRender;
  return (ik3UNIT *)Uni;
} /* End of 'IK3_UnitCreateCube' function */

/* END OF 'U_CUBE.C' FILE */
 