/* FILE NAME: U_BALL.C
 * PROGRAMMER: IK3
 * DATE: 11.06.2016
 * PURPOSE: Ball unit sample.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#pragma warning(disable: 4244)
#define MAX_UNITS 10000

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
  DBL
    GlobalTime, GlobalDeltaTime,
    Time, DeltaTime,
    FPS;
  BOOL IsPause;
  INT 
    Mx, My,
    Mz, 
    Mdx, Mdy, Mdz;
  BYTE 
    Keys[256],
    OldKeys[256],
    KeysClick[256];
  BYTE
  JBut[32];                      /* Joystick button state */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL JX, JY, JZ, JR; 
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
VOID IK3_AnimUnit( HWND hWnd );
VOID IK3_AnimAddUnit( ik3UNIT *Uni );
VOID IK3_AnimClose( VOID );
VOID IK3_Reasize( INT W, INT H );
VOID IK3_AnimRender( VOID );
VOID IK3_AnimDoExit( VOID );
ik3UNIT * IK3_AnimUnitCreate( INT Size );
VOID IK3_AnimFlipFullScreen( VOID );

#endif
