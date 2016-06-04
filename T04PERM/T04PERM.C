/* FILE NAME: T04CLOCK.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <stdio.h>
#define N 3
int P[N] = {1, 2, 3};

int Parity( void )
{
  int i, j, sum = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N - i; j++)
      if (P[i] > P[i + j])
        sum++;
  
  return sum;
}
void Swap( int *A, int *B )
{
  int tmp = *A;
  
  *A = *B;
  *B = tmp;
}
void SavePerm( void )
{ 
  FILE *F;
  int i;

  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N - 1; i++)
    fprintf(F, "%2i,", P[i]);
  fprintf(F, "%2i", P[N - 1]);
  if (Parity() % 2 == 0)
    fprintf(F, " - even\n");
  else 
    fprintf(F, " - odd\n");
  fclose(F);
}
void Go( int Pos )
{
  int i;
  
  if (Pos == N)
  {
    SavePerm();
    return; 
  }
  else
  {
    for (i = Pos; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
    }
  } 
}
void main( void )
{
  Go(0);
}
