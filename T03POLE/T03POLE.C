/* FILE NAME: T01EYES.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
/* The start PolygonTriangle function */
VOID PolygonTriangle( HWND hWnd, HDC hDC, INT X, INT Y )
{
  POINT pt; 
  static POINT pts[] =
  {
    {0, -20}, {40, 0}, {0, 20}
  },
  pts1[] =
  {
     {0, 20}, {-40, 0}, {0, -20}
  };
  POINT pts2[sizeof(pts) / sizeof(pts[0])], pts3[sizeof(pts1) / sizeof(pts1[0])];
  INT i;
  DOUBLE co, si;
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
                                                                                  
  si = (Y - pt.y) / sqrt((Y - pt.y) * (Y - pt.y) + (pt.x - X ) * (pt.x - X));
  co = (pt.x - X) / sqrt((Y - pt.y) * (Y - pt.y) + (pt.x - X ) * (pt.x - X));
                                      
  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  { 
    pts2[i].x = X + pts[i].x * co - pts[i].y * si;
    pts2[i].y = Y - (pts[i].x * si + pts[i].y * co);
    pts3[i].x = X + pts1[i].x * co - pts1[i].y * si;
    pts3[i].y = Y - (pts1[i].x * si + pts1[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  srand(clock() / 1000);
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(rand() * 5, rand()  / 500, rand() * 3));
  Polygon(hDC, pts2, sizeof(pts) / sizeof(pts[0]));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(rand() /50, rand() * 80 / 1000, rand() * 3));
  Polygon(hDC, pts3, sizeof(pts1) / sizeof(pts1[0])); 
  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
 }/* The end PolygonTriangle function */
/* The start of 'MyWinFunc' function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i, j;
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
  case WM_KEYDOWN:
    if (LOWORD(wParam) == 'F')
      FlipFullScreen(hWnd);
    if (LOWORD(wParam) == VK_ESCAPE)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    for (i = 0; i < 100; i++)
      for (j = 0; j < 60; j++)
        PolygonTriangle(hWnd, hMemDC, 100 + i * 12 , 200 + j * 12);
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
