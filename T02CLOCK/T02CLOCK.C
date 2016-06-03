/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <windows.h>
#include <math.h>

/* The start of 'MyWinFunc' function */
VOID PutLineTime( HDC hDC, INT X, INT Y, INT X1, INT Y1 )
{
  MoveToEx(hDC, X, Y, NULL); 
  LineTo(hDC, X1, Y1);
}
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT x = 512, y = 512;
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
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    srand(59);
    for (i = 0; i < 500; i++)
      DrawEye(hWnd, hMemDC, rand() % 2000, rand() % 1000, 50, 15);
    BitBlt(hMemDC, 10, 1, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);

    hPen = CreatePen(PS_SOLID, 7, RGB(0, 0, 0));
    hOldPen = SelectObject(hMemDC, hPen);
    GetLocalTime(&st);
    t = st.wSecond / 60.0 * 2 * P + st.wMilliseconds / 1000;
    si = sin(t);
    co = cos(t);
    PutLineTime(hMemDC, w /  , h / 2, w / 2 + si * r, h / 2 - co * r);
    t = st.wMinute / 60.0 * 2.0 * P + st.wSecond / 60;
    si = sin(t);
    co = cos(t);
    PutLineTime(hMemDC, w / 2, h / 2, w / 2 + 300 * si, h / 2 - 300 * co);
    t = st.wHour / 12.0 * 2.0 * P;
    si = sin(t);
    co = cos(t);
    PutLineTime(hMemDC, w / 2, h / 2, w / 2 + 280 * si, h / 2 - 280 * co);
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
  hWnd = CreateWindow("My Window Class", "30!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1044, 1044, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  
  return 30;
}
/* The end of 'WinMain' function */
