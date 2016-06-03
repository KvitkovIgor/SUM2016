/* FILE NAME: T01EYES.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <windows.h>
#include <math.h>
#include <stdlib.h>

/* The start of drawing eye function */
/* The start PolygonTriangle function */
VOID PolygonTriangle( HWND hWnd, HDC hDC, INT X, INT Y )
{
  POINT pt; 
  static POINT pts[] =
  {
    {100, 200}, {50, 350}, {150, 350}
  }; 
  POINT pts1[sizeof(pts) / sizeof(pts[0])];
  INT dx, dy, i;
  DOUBLE co, si;
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  dx = pt.x - X;
  dy = pt.y - Y;
  si = sin((pt.y - Y) / sqrt((pt.y - Y) * (pt.y - Y) + (pt.x - X) * (pt.x - X)));
  co = cos((pt.x - X) / sqrt((pt.y - Y) * (pt.y - Y) + (pt.x - X) * (pt.x - X)));
  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  { 
    pts1[i].x = X + pts[i].x * co - pts[i].y * si;
    pts1[i].y = Y - (pts[i].x * si - pts[i].y * co);
  }

  Polygon(hDC, pts1, sizeof(pts) / sizeof(pts[0])); 
 }/* The end PolygonTriangle function */
/* The start of 'MyWinFunc' function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  static INT w, h;
  static HBITMAP hBm;
  static HDC hMemDC;
  HBRUSH hBr, hOldBr;

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
    hBr = CreateSolidBrush(RGB(255, 255, 0));
    hOldBr = SelectObject(hMemDC, hBr);
    PolygonTriangle(hWnd, hMemDC, 100, 100);
    SelectObject(hMemDC, hOldBr);
    DeleteObject(hBr);
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
