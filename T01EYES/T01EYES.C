/* FILE NAME: T01EYES.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <windows.h>
#include <math.h>
#include <stdlib.h>

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
  a = (R - R1) /sqrt(dx * dx + dy * dy);
  if (a < 1)
  {
    dx *= a;
    dy *= a;
  }
  Ellipse(hDC, X + dx - R1, Y + dy - R1, X + dx + R1, Y + dy + R1); 
}/* The end of drawing eye function */
/* The start of 'MyWinFunc' function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i;
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h;
  static HBITMAP hBm;
  static HDC hMemDC;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
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
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    srand(59);
    for (i = 0; i < 1000; i++)
      DrawEye(hWnd, hMemDC, rand() % 2000, rand() % 1000, 50, 15);
    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(255, 0, 255));
    TextOut(hMemDC, 5, 5, "THE PROGRAMMER IN THE WORLD", 15);
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
  hWnd = CreateWindow("My Window Class", "THE CGSG PROGA", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1000, 1900, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  
  return 30;
}
/* The end of 'WinMain' function */
