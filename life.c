
/* 2016.04.14 
   the famous life game .
*/

#include <stdlib.h>
#include <stdio.h>

int width = 10 , height = 5 ;
int steps = 3 , wrap = 1 ;

void update(int **sum )
{
}

int main(int argc , char **argv)
{
  int **states , **sum , **newsum;
  int i , j ;

  printf("hello , life game\n");

  states = malloc(sizeof(int*)*height);
  sum = malloc(sizeof(int*)*height);
  newsum = malloc(sizeof(int*)*height);

  for(i = 0 ; i < height ; i++)
  {
    states[i]=malloc(sizeof(int)*width);
    sum[i]= malloc(sizeof(int)*width);
    newsum[i] = malloc(sizeof(int)*width);

    for(j=0;j<width;j++)
      states[i][j]=sum[i][j]=0;
  }

  for(i = 0 ; i < height ; i++)
  {
    for(j = 0 ; j < width ; j++)
    {

    }
  }

  printf("finish\n");
  exit(1);
}
