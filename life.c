
/* 2016.04.14 , the famous life game . */

#include <stdlib.h>
#include <stdio.h>

int width = 10 , height = 5 ;
int steps = 3 , wrap = 1 ;

void updatecount(int **sum , int rw , int rh , int ii , int jj , 
                 int wrap , int change )
{
}

int main(int argc , char **argv)
{
  int **states , **sum , **newsum , **swap;
  int s , i , j  , change ;

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

  for(s = 0 ; s < steps ; s++)
  {
    for(i = 0 ; i < height ; i++)
    {
      for(j = 0 ; j < width ; j++)
      {
        /* rules */
        if(states[i][j]==0 && sum[i][j]==3)
	{
          updatecount(newsum,width,height,i,j,wrap,1);
          states[i][j] = 1 ;
          change = 1 ;
	}
        else if(states[i][j]==1 && (sum[i][j]<2||sum[i][j]>3))
	{
          updatecount(newsum,width,height,i,j,wrap,-1);
          states[i][j] = 0 ;
          change = 1 ;
	}
      } /* for(j = 0 ; j < width ;*/
    } /* for(i = 0 ; i < height ; */
    swap = sum ; sum = newsum ; newsum = swap ; 
  } /* for(s = 0 ; s < steps ; */

  printf("finish\n");
  exit(1);
}
