/* FILE NAME: T04CLOCK.C
 * PROGRAMMER: IK3
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include <stdio.h>
#define N 3
int P[N] = {1, 2, 3}, Parity = 0;


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
  if (Parity)
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
      if (P[Pos] > P[i])
        Parity = !Parity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
    }
  } 
}
void main( void )
{
  Go(0);
}
