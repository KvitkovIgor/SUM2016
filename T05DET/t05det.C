/* FILE NAME: T04CLOCK.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <stdio.h>
#include <conio.h>
#define N 5  
#define MAX 30
int P[N], Parity = 0;
double sum, A[MAX][MAX];

void Swap( int *A, int *B )
{
  int tmp = *A;
  
  Parity = !Parity;

  *A = *B;
  *B = tmp;
}

void LoadMatrix( char *FileName )
{
  int i, j;
  FILE *F;

  F = fopen(FileName, "r");
  if (F == NULL)
    return;
  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
      fscanf(F, "%lf", A[i][j]);
      
  fclose(F);
}
void Go( int Pos )
{
  int i, prod = 1, SaveParity;
  double x;
  
  if (Pos == N)
  {                           
    for (i = Pos; i < MAX; i++)
      prod *= A[i][P[i]];
    if (Parity)
      sum += prod;
    else
      sum -= prod;
    return; 
  }
  else
  {
    x = P[0];
    Go(Pos + 1);
    SaveParity = Parity;
    for (i = Pos; i < MAX; i++)
    {
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
    }
  } 
  x = P[0];
   for (i = Pos + 1; i < N; i++)
    P[i - 1] = P[i];
  P[N - 1] = x;
  Parity = SaveParity;

}
double EvalDeterminat( char *FileName )
{
  int i;
  
  LoadMatrix(FileName);
  sum = 0;

  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  return sum;
}
void main( void ) 
{
  printf("DET = %f\n", EvalDeterminat("m3.txt"));
  _getch();
}
