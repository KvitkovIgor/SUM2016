/* FILE NAME: U_CLOCK.C
 * PROGRAMMER: IK3
 * DATE: 11.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */
#include <time.h>

#include "anim.h"
#include "untis.h"

typedef struct
{
  ik3UNIT;
} ik3UNIT_CLOCK;

static VOID IK3_UnitInit( ik3UNIT_CLOCK *Uni, ik3Anim *Ani )
{
}
static VOID IK3_UnitResponse( ik3PRIM *Uni, ik3Anim *Ani )
{
  
  if (Ani->KeysClick[VK_ESCAPE])
    IK3_AnimDoExit();
}
static VOID IK3_UnitRender( ik3UNIT_CLOCK *Uni, ik3Anim *Ani )
{
  DBL a, r;
  SYSTEMTIME st;

  GetLocalTime(&st);

  a = (st.wSecond) * 2 * PI / 60;
  r = Ani->W / 2.2;

  if (Ani->JBut[0])
    SetDCPenColor(Ani->hDC, RGB(255, 0, 0));
  else
    SetDCPenColor(Ani->hDC, RGB(0, 0, 0));

  MoveToEx(Ani->hDC, Ani->W / 2 + Ani->JX * 30, Ani->H / 2 + Ani->JY * 30, NULL);
  MoveToEx(Ani->hDC, Ani->W / 2 + Ani->JX * 30 + sin(a) * r, Ani->H / 2 + Ani->JY * 30 - cos(a) * r, NULL);
}
ik3UNIT * IK3_UnitCreateClock( VOID )
{
  ik3UNIT_CLOCK *Uni;

  if ((Uni = (ik3UNIT_CLOCK *)IK3_AnimUnitCreate(sizeof(ik3UNIT_CLOCK))) == NULL)
    return NULL;
  Uni->Init = (VOID *)IK3_UnitInit;
  Uni->Response = (VOID *)IK3_UnitResponse;
  Uni->Render = (VOID *)IK3_UnitRender;
  return (ik3UNIT *)Uni;
}
