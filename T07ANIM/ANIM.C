/* FILE NAME: VEC.H
 * PROGRAMMER: IK3
 * DATE: 08.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */ 
#include "anim.h"
#include "untis.h"
#include <time.h>

#include <mmsystem.h> 
#pragma comment(lib, "winmm")
#define IK3_GET_JOYSTIC_AXIS(A)\
  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

ik3Anim IK3_Anim;
HDC hMemDClogo;

VOID IK3_AnimInit( HWND hWnd )
{
  HDC hDC;
  static HBITMAP hBmLogo;
  static BITMAP bm;

  hBmLogo = LoadImage(NULL, "N.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  GetObject(hBmLogo, sizeof(bm), &bm);
  hDC = GetDC(hWnd);
  IK3_Anim.hDC = CreateCompatibleDC(hDC);
  hMemDClogo = CreateCompatibleDC(hDC);    
  IK3_Anim.NumOfUnits = 0, memset(&IK3_Anim, 0, sizeof(ik3Anim));
  IK3_Anim.hWnd = hWnd;
  IK3_Anim.hDC = CreateCompatibleDC(hDC);
  SelectObject(hMemDClogo, hBmLogo);
  ReleaseDC(hWnd, hDC);

}
VOID IK3_Reasize( INT W, INT H )
{
  HDC hDC;
  
  IK3_Anim.W = W;
  IK3_Anim.H = H;

  if (IK3_Anim.hFrame != NULL)
    DeleteObject(IK3_Anim.hFrame);
  hDC = GetDC(IK3_Anim.hWnd);
  IK3_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(IK3_Anim.hWnd, hDC);
  SelectObject(IK3_Anim.hDC, IK3_Anim.hFrame);
}
VOID IK3_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, IK3_Anim.W, IK3_Anim.H, IK3_Anim.hDC, 0, 0, SRCCOPY);
}
VOID IK3_AnimAddUnit( ik3UNIT *Uni )
{
  if (IK3_Anim.NumOfUnits < MAX_UNITS)
  {  
    IK3_Anim.Units[IK3_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &IK3_Anim);
  }
}
VOID IK3_AnimDoExit( VOID )
{
  DestroyWindow(IK3_Anim.hWnd);
}
VOID IK3_AnimClose( VOID )
{
  int i;

  for (i = 0; i < IK3_Anim.NumOfUnits; i++)
  {
    IK3_Anim.Units[i]->Close(IK3_Anim.Units[i], &IK3_Anim);
    free(IK3_Anim.Units[i]);
  }
  DeleteDC(IK3_Anim.hDC);
  DeleteObject(IK3_Anim.hFrame);
  IK3_Anim.NumOfUnits = 0;
}
/* The start of 'PutLinetime' function */
VOID PutLineTime( HDC hDC, INT X, INT Y, INT X1, INT Y1 )
{
  MoveToEx(hDC, X, Y, NULL); 
  LineTo(hDC, X1, Y1);
}/* The end of 'PutLinetime' function */
VOID IK3_AnimRender( VOID )
{
  int i; 
  
  /* Joystick */

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
          IK3_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* Axes */
        IK3_Anim.JX = IK3_GET_JOYSTIC_AXIS(X);
        IK3_Anim.JY = IK3_GET_JOYSTIC_AXIS(Y);
        IK3_Anim.JZ = IK3_GET_JOYSTIC_AXIS(Z);
        IK3_Anim.JR = IK3_GET_JOYSTIC_AXIS(R);

        /* Point of view */
        IK3_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  for (i = 0; i < IK3_Anim.NumOfUnits; i++)
    IK3_Anim.Units[i]->Response(IK3_Anim.Units[i], &IK3_Anim);

  
  SelectObject(IK3_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(IK3_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(IK3_Anim.hDC, RGB(255, 255, 255)); 
  Rectangle(IK3_Anim.hDC, 0, 0, IK3_Anim.W, IK3_Anim.H);
  
  DrawSphere( IK3_Anim.hDC, 1000 + IK3_Anim.JX * 700, 512 + IK3_Anim.JY * 700, 100);
  BitBlt(IK3_Anim.hDC, 1500, 0, IK3_Anim.W, IK3_Anim.H, hMemDClogo, 0, 0, SRCAND); 

  for (i = 0; i < IK3_Anim.NumOfUnits; i++)
  {
    IK3_Anim.Units[i]->Render(IK3_Anim.Units[i], &IK3_Anim);
  }
}

