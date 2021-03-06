/* FILE NAME: T04CLOCK.C
 * PROGRAMMER: IK3
 * DATE: 06.06.2016
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
    fprintf(F, " - odd\n");
  else 
    fprintf(F, " - even\n");
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
      if (Pos != i)
        Swap(&P[Pos], &P[i]), Parity = !Parity;
      Go(Pos + 1);
      if (Pos != i)
        Swap(&P[Pos], &P[i]), Parity = !Parity;
    }
  } 
}
void main( void )
{
  Go(0);
}
