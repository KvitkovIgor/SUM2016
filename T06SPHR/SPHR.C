/* FILE NAME: SPHR.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */
#include "sphr.h"
#define M 15
#define N 30

typedef double DBL;

typedef struct
{
  DBL X, Y, Z;
} VEC;


VEC Rot( VEC V, DBL Angle )
{
  VEC r;
  DBL A = Angle * pi / 180;
  
  r.X = V.X;
  r.Y = V.Y * cos(A) - V.Z * sin(A);
  r.Z = V.Y * sin(A) - V.Z * cos(A);
  
  return r;
}
VOID DrawSphere( HDC hDC, INT Xc, INT Yc )
{
  INT i, j, R = 400, x, y;
  DBL theta, phi;
  static VEC G[M][N];
  
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(90, 90, 90));

  for (i = 0; i < M; i++)
  {
    theta = i * pi / (M - 1);
    for (j = 0; j < N; j++)
    {
      phi = j * 2 * pi / (N - 1) + clock() / 10000.0;
      G[i][j].X =  R * sin(theta) * cos(phi);
      G[i][j].Y =  R * sin(theta) * sin(phi);
      G[i][j].Z =  R * cos(theta); 
    
      G[i][j] = Rot(G[i][j], 30 * sin(clock() / 1000.0));
    }
  }

  for (i = 0; i < M; i++)
  {
    for (j = 0; j < N; j++)
    {
      x = Xc + G[i][j].X;
      y = Yc + G[i][j].Z;

      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);

    }
  }

  for (i = 0; i < M; i++)
  {
    x = Xc + G[i][0].X;
    y = Yc - G[i][0].Z;
    MoveToEx(hDC, x, y, NULL);
    for (j = 1; j < N; j++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z; 
     
      LineTo(hDC, x, y);
    }
  }

  for (j = 0; j < N; j++)
  {
    x = Xc + G[0][j].X;
    y = Yc - G[0][j].X;
    MoveToEx(hDC, x, y, NULL);
    for (i = 1; i < M; i++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z; 
      
      LineTo(hDC, x, y);
    }
   }

  SetDCPenColor(hDC, RGB(255, 255, 255));
}
