/* FILE NAME: U_BALL.C
 * PROGRAMMER: IK3
 * DATE: 11.06.2016
 * PURPOSE: Ball unit sample.
 */

#include "anim.h"
#include "units.h"

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(hWnd, HWND_TOP,
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
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
  IK3_RndSetProj();
} /* End of 'FlipFullScreen' function */

/* The start of 'MyWinFunc' function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *MinMax;
  INT w, h;
  SetDbgMemHooks();

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax->ptMaxTrackSize.y = 
     GetSystemMetrics(SM_CYMAXTRACK) +
     GetSystemMetrics(SM_CYCAPTION) +
     GetSystemMetrics(SM_CYMENU) +
     GetSystemMetrics(SM_CYBORDER) * 2;
  case WM_CREATE:
    SetTimer(hWnd, 30, 2, NULL);
    IK3_AnimUnit(hWnd); 
    SendMessage(hWnd, WM_SIZE, 0, 0);
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    /* Animation resize */
    IK3_Reasize(w, h);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEWHEEL:
    IK3_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == 'F')
      FlipFullScreen(hWnd);
    if (LOWORD(wParam) == VK_ESCAPE)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER: 
    IK3_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    IK3_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    IK3_AnimClose();
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);

} /* End of 'MyWinFunc' function */
/* The start of 'WinMain' function */
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  MSG msg;
  HWND hWnd;
  WNDCLASS wc;


  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = "My Window Class";
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  RegisterClass(&wc);
  hWnd = CreateWindow("My Window Class", "THE CGSG PROGA", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);
  /* Update window */
  UpdateWindow(hWnd);

    IK3_AnimAddUnit(IK3_UnitCreateControl());
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  
  return 30;
}
/* The end of 'WinMain' function */
