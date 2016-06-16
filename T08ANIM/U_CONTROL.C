/* FILE NAME: U_CONTROL.C
 * PROGRAMMER: IK3
 * DATE: 15.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <time.h>


#include "units.h"
#include "render.h"

typedef struct
{
  ik3UNIT;
  VEC Pos;
} ik3UNIT_CONTROL;

static VOID IK3_UnitInit( ik3UNIT_CONTROL *Uni, ik3Anim *Ani )
{
   Uni->Pos = VecSet(5, 0, 5);
}
static VOID IK3_UnitResponse( ik3UNIT_CONTROL *Uni, ik3Anim *Ani )
{
  DBL r;

  if (Ani->Keys['T'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['Y'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['U'])
    glPolygonMode(GL_BACK, GL_LINE);
  if (Ani->Keys[VK_SPACE])
    IK3_AnimAddUnit(IK3_UnitCreateBall());
  if (Ani->KeysClick['C'])
    IK3_AnimAddUnit(IK3_UnitCreateCube());
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    IK3_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    IK3_AnimDoExit();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime; */
  Uni->Pos = PointTransform4(Uni->Pos, MatrRotateX(1.59 * Ani->JY ));
  Uni->Pos = PointTransform4(Uni->Pos, MatrRotateY(1.59 * Ani->JX ));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = PointTransform4(Uni->Pos, MatrRotateY(1.59  * Ani->Mdx));
    Uni->Pos = PointTransform4(Uni->Pos, MatrRotateX(1.59  * Ani->Mdy ));
  }

  Uni->Pos = PointTransform4(Uni->Pos, MatrRotateY(0.059  * Ani->Keys[VK_RIGHT]));
  Uni->Pos = PointTransform4(Uni->Pos, MatrRotateY(-0.059  * Ani->Keys[VK_LEFT]));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + Ani->Mdz * Ani->DeltaTime * 0.1) / r);

  IK3_RndMatrView = MatrView(Uni->Pos, VecSet(2, 2, 2), VecSet(0, 1, 0));

} /* End of 'IK3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       IK3UNIT_CUBE *Uni;
 *   - animation context:
 *       IK3ANIM *Ani;
 * RETURNS: None.
 */
static VOID IK3_UnitRender( ik3UNIT_CONTROL *Uni, ik3Anim *Ani )
{
}
ik3UNIT * IK3_UnitCreateControl( VOID )
{
  ik3UNIT_CONTROL *Uni;

  if ((Uni = (ik3UNIT_CONTROL *)IK3_AnimUnitCreate(sizeof(ik3UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)IK3_UnitInit;
  Uni->Response = (VOID *)IK3_UnitResponse;
  return (ik3UNIT *)Uni;
}