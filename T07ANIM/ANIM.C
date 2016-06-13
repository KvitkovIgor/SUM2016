/* FILE NAME: VEC.H
 * PROGRAMMER: IK3
 * DATE: 08.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */ 

#include <stdio.h>
#include "anim.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

#define IK3_GET_JOYSTIC_AXIS(A) \
  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

INT IK3_MouseWheel;

ik3Anim IK3_Anim;

static UINT64
  IK3_StartTime, 
  IK3_OldTime, 
  IK3_OldTimeFPS, 
  IK3_PauseTime, 
  IK3_TimePerSec, 
  IK3_FrameCounter;

VOID IK3_AnimUnit( HWND hWnd )
{
  HDC hDC; 
  LARGE_INTEGER t;

  memset(&IK3_Anim, 0, sizeof(ik3Anim));

  IK3_Anim.hWnd = hWnd;
  hDC  = GetDC(hWnd);
  IK3_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  QueryPerformanceFrequency(&t);
  IK3_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  IK3_StartTime = IK3_OldTime = IK3_OldTimeFPS = t.QuadPart;
  IK3_PauseTime = 0;
}
VOID IK3_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(IK3_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(IK3_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(IK3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(IK3_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(IK3_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */
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
VOID IK3_AnimRender( VOID )
{
  int i;
  POINT pt;
  LARGE_INTEGER t;

  IK3_FrameCounter++;
  QueryPerformanceCounter(&t);

  IK3_Anim.GlobalTime =(DBL)(t.QuadPart - IK3_StartTime) / IK3_TimePerSec;
  IK3_Anim.GlobalDeltaTime =(DBL)(t.QuadPart - IK3_OldTime) / IK3_TimePerSec;
  if (IK3_Anim.IsPause)
  {
    IK3_Anim.DeltaTime = 0;
    IK3_PauseTime += t.QuadPart - IK3_OldTime;
  }
  else 
  {
    IK3_Anim.DeltaTime = IK3_Anim.GlobalDeltaTime;
    IK3_Anim.Time = (DBL)(t.QuadPart - IK3_PauseTime - IK3_OldTime) / IK3_TimePerSec;
  }

  if (t.QuadPart - IK3_OldTimeFPS > IK3_TimePerSec);
  {
    CHAR str[100];

    IK3_Anim.FPS = IK3_FrameCounter * IK3_TimePerSec / (DBL)(t.QuadPart - IK3_OldTimeFPS);
    IK3_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS: %.5f", IK3_Anim.FPS);
    SetWindowText(IK3_Anim.hWnd, str);
    IK3_FrameCounter = 0;
  }

  GetKeyboardState(IK3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    IK3_Anim.Keys[i] >>= 7;
    if (!IK3_Anim.OldKeys[i] && IK3_Anim.Keys[i])
      IK3_Anim.KeysClick[i] = TRUE;
    else
      IK3_Anim.KeysClick[i] = FALSE;
  }
  memcpy(IK3_Anim.OldKeys, IK3_Anim.Keys, 256);

  GetCursorPos(&pt);
  ScreenToClient(IK3_Anim.hWnd, &pt);
  IK3_Anim.Mdx = pt.x - IK3_Anim.Mx;
  IK3_Anim.Mdy = pt.y - IK3_Anim.My;
  IK3_Anim.Mdx = pt.x;
  IK3_Anim.Mdy = pt.y;

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
  SetDCBrushColor(IK3_Anim.hDC, RGB(100, 155, 210)); 
  Rectangle(IK3_Anim.hDC, 0, 0, IK3_Anim.W, IK3_Anim.H);
  
  for (i = 0; i < IK3_Anim.NumOfUnits; i++)
  {
    SelectObject(IK3_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(IK3_Anim.hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(IK3_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(IK3_Anim.hDC, RGB(0, 0, 0));

    IK3_Anim.Units[i]->Render(IK3_Anim.Units[i], &IK3_Anim);
  }
}





