#ifndef __ANIM_H_
#define __ANIM_H_

#pragma warning(disable: 4244)
#define MAX_UNITS 5

#include "def.h"

typedef struct tagik3UNIT ik3UNIT;
typedef struct tagik3Anim
{
  HWND hWnd;
  HDC hDC;  
  INT W, H;  
  HBITMAP hFrame;
  ik3UNIT *Units[MAX_UNITS];
  INT NumOfUnits;
} ik3Anim;
struct tagik3UNIT
{
  VOID (*Init)( ik3UNIT *Uni, ik3Anim *Ani );
  VOID (*Close)( ik3UNIT *Uni, ik3Anim *Ani );
  VOID (*Response)( ik3UNIT *Uni, ik3Anim *Ani );
  VOID (*Render)( ik3UNIT *Uni, ik3Anim *Ani );
};

extern ik3Anim IK3_Anim;

VOID IK3_AnimCopyFrame( HDC hDC );
VOID IK3_AnimInit( HWND hWnd );
VOID IK3_AnimAddUnit( ik3UNIT *Uni );
VOID IK3_AnimClose( VOID );
VOID IK3_Reasize( INT W, INT H );
VOID IK3_AnimRender( VOID );
VOID IK3_AnimDoExit( VOID );
ik3UNIT * IK3_AnimUnitCreate( INT Size );

#endif
