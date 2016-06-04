/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <windows.h>
#include <math.h>
#include <stdlib.h>

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
} /* End of 'FlipFullScreen' function */
/* The start of drawing eye function */
VOID DrawEye(HWND hWnd, HDC hDC, INT X, INT Y, INT R, INT R1)
{
  POINT pt;
  INT dx, dy;
  DOUBLE a;
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  Ellipse(hDC, X - R, Y - R, X + R, Y + R); 
  dx = pt.x - X;
  dy = pt.y - Y;
  a = (R - R1) / sqrt(dx * dx + dy * dy);
  if (a < 1)
  {
    dx *= a;
    dy *= a;
  }
  Ellipse(hDC, X + dx - R1, Y + dy - R1, X + dx + R1, Y + dy + R1); 
}/* The end of drawing eye function */
/* The start of 'PutLinetime' function */
VOID PutLineTime( HDC hDC, INT X, INT Y, INT X1, INT Y1 )
{
  MoveToEx(hDC, X, Y, NULL); 
  LineTo(hDC, X1, Y1);
}/* The end of 'PutLinetime' function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT x = 512, y = 512, i;
  SYSTEMTIME st;
  HDC hDC;
  HPEN hPen, hOldPen;
  PAINTSTRUCT ps;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  DOUBLE P = 3.14159265358979323846, t, si, co, r = 400;

  GetLocalTime(&st);


  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 2, NULL);
    hBmLogo = LoadImage(NULL, "S.BMP", IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDCLogo, hBmLogo);
    ReleaseDC(hWnd, hDC);
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == 'F')
      FlipFullScreen(hWnd);
    if (LOWORD(wParam) == VK_ESCAPE)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    srand(59);
    
    
    for (i = 0; i < 300; i++)
      DrawEye(hWnd, hMemDC, rand() % 2000, rand() % 1000, 100, 20);
    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);
    DrawEye(hWnd, hMemDC, 512, 512, 370, 50);
    hPen = CreatePen(PS_SOLID, 30, RGB(127, 127, 127));
    hOldPen = SelectObject(hMemDC, hPen);
    GetLocalTime(&st);
    t = (st.wSecond  + st.wMilliseconds / 1000.0 ) / 60.0 * 2 * P;
    si = sin(t);
    co = cos(t);
    PutLineTime(hMemDC, 512, 512, 512 + si * r, 512 - co * r);
    t = (st.wMinute + st.wSecond / 60.0) / 60.0 * 2.0 * P + st.wSecond / 60;
    si = sin(t);
    co = cos(t);
    PutLineTime(hMemDC, 512, 512, 512 + 300 * si, 512 - 300 * co);
    t = (st.wHour + st.wMinute / 60.0) / 12.0 * 2.0 * P;
    si = sin(t);
    co = cos(t);
    PutLineTime(hMemDC, 512, 512, 512 + 280 * si, 512 - 280 * co);
    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);                                                   
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DeleteObject(hBm);
    DeleteObject(hMemDC); 
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */
/* The start of 'WinMain' function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
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
  hWnd = CreateWindow("My Window Class", "30!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1044, 1044, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  
  return 30;
}
/* The end of 'WinMain' function */
