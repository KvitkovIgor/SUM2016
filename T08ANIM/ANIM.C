/* FILE NAME: VEC.H
 * PROGRAMMER: IK3
 * DATE: 08.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */ 

#include <stdio.h>
#include "anim.h"
#include "units.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

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
  INT i;
  LARGE_INTEGER t;
  PIXELFORMATDESCRIPTOR pfd = {0};
 
  memset(&IK3_Anim, 0, sizeof(ik3Anim));

  /* Store window and create memory device context */
  IK3_Anim.hWnd = hWnd;
  IK3_Anim.hDC = GetDC(hWnd);
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(IK3_Anim.hDC, &pfd);
  DescribePixelFormat(IK3_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(IK3_Anim.hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  IK3_Anim.hGLRC = wglCreateContext(IK3_Anim.hDC);
  wglMakeCurrent(IK3_Anim.hDC, IK3_Anim.hGLRC);

  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(IK3_Anim.hGLRC);
    ReleaseDC(IK3_Anim.hWnd, IK3_Anim.hDC);
    exit(0);
  }

  QueryPerformanceFrequency(&t);
  IK3_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  IK3_StartTime = IK3_OldTime = IK3_OldTimeFPS = t.QuadPart;
  IK3_PauseTime = 0;

  IK3_RndMatrWorld  = MatrIdentity();
  IK3_RndMatrView = MatrView(VecSet(0, 1, 0), VecSet(0, 0, 0), VecSet(0, 1, 0));
  IK3_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);
  IK3_RndProjDist = 3;
  IK3_RndFarClip = 2000;
  IK3_RndProjSize = 3;
  /* OpenGL specific initialization */
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
  IK3_RndPrg = IK3_RndShaderLoad("a");
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
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
  IK3_Anim.W = W;
  IK3_Anim.H = H;

  glViewport(0, 0, IK3_Anim.W, IK3_Anim.H);
  IK3_RndSetProj();
}
VOID IK3_AnimCopyFrame( HDC hDC )
{
  SwapBuffers(IK3_Anim.hDC);
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
  
  IK3_RndShaderFree(IK3_RndPrg);
  /* Delete rendering context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(IK3_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(IK3_Anim.hWnd, IK3_Anim.hDC);

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
  { 
    IK3_Anim.Units[i]->Response(IK3_Anim.Units[i], &IK3_Anim);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*** Render all units ***/
  for (i = 0; i < IK3_Anim.NumOfUnits; i++)
  {
    IK3_RndMatrWorld = MatrIdentity();
    IK3_Anim.Units[i]->Render(IK3_Anim.Units[i], &IK3_Anim);
  }

  /* Finalize OpenGL drawing */
  glFinish();
}