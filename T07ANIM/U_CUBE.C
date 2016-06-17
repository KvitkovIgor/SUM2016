/* FILE NAME: U_CUBE.C
 * PROGRAMMER: IK3
 * DATE: 11.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */
#include <time.h>
#include "render.h"
#include "units.h"

/* Ball unit representation type */
typedef struct
{
  ik3UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  ik3PRIM Pr;
  DBL TimerSpeed;  /* Timer speed value*/
} ik3UNIT_CUBE;


/* Cube points */
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

/* Cube edges */
INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

/* Cube primitive */
ik3PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

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
  Uni->Pos = VecSet(Rnd0() * 5, Rnd0() * 5, Rnd0() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  IK3_RndPrimLoad(&Uni->Pr, "g3d\\cow.g3d");
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
   IK3_RndPrimFree(&Uni->Pr);
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
  if (Ani->KeysClick[VK_SPACE])
    IK3_AnimAddUnit(IK3_UnitCreateBall());
  if (Ani->KeysClick[VK_F1])
    IK3_AnimAddUnit(IK3_UnitCreateCube());
  if (Ani->KeysClick[VK_F2])
    IK3_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    IK3_AnimDoExit();
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
  IK3_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.6F, 0.6F, 0.6F)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                                         VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3)))));
  IK3_RndPrimDraw(&Uni->Pr);
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
 