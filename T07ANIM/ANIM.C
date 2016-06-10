/* FILE NAME: VEC.H
 * PROGRAMMER: IK3
 * DATE: 08.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */ 
#include "anim.h"

ik3Anim IK3_Anim;

VOID IK3_AnimInit( HWND hWnd )

{
  HDC hDC;

  IK3_Anim.NumOfUnits = 0, memset(&IK3_Anim, 0, sizeof(ik3Anim));
  IK3_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  IK3_Anim.hDC = CreateCompatibleDC(hDC);
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
  SYSTEMTIME st;
  HPEN hPen, hOldPen;
  DOUBLE P = 3.14159265358979323846, t, si, co, r = 400; 
  
    
  for (i = 0; i < IK3_Anim.NumOfUnits; i++)
    IK3_Anim.Units[i]->Response(IK3_Anim.Units[i], &IK3_Anim);

  SelectObject(IK3_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(IK3_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(IK3_Anim.hDC, RGB(255, 255, 255)); 
  Rectangle(IK3_Anim.hDC, 0, 0, IK3_Anim.W, IK3_Anim.H);
  
  hPen = CreatePen(PS_SOLID, 30, RGB(100, 155, 220));
  hOldPen = SelectObject(IK3_Anim.hDC, hPen);
  GetLocalTime(&st);

  t = (st.wSecond  + st.wMilliseconds / 1000.0 ) / 60.0 * 2 * P;
  si = sin(t);
  co = cos(t);
  PutLineTime(IK3_Anim.hDC, 512, 512, 512 + si * r, 512 - co * r);
  t = (st.wMinute + st.wSecond / 60.0) / 60.0 * 2.0 * P + st.wSecond / 60;
  si = sin(t);
  co = cos(t);
  PutLineTime(IK3_Anim.hDC, 512, 512, 512 + 300 * si, 512 - 300 * co);
  t = (st.wHour + st.wMinute / 60.0) / 12.0 * 2.0 * P;
  si = sin(t);
  co = cos(t);
  PutLineTime(IK3_Anim.hDC, 512, 512, 512 + 280 * si, 512 - 280 * co);
  SelectObject(IK3_Anim.hDC, hOldPen);
  DeleteObject(hPen);
   

  for (i = 0; i < IK3_Anim.NumOfUnits; i++)
  {
    IK3_Anim.Units[i]->Render(IK3_Anim.Units[i], &IK3_Anim);
  }
}

