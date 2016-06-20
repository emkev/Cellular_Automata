
/* 20151202 23:32 start . Plot cos-curve with pringf() .  */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main()
{
  double y ;
  int x , m ;
  
  for(y = 1 ; y >= -1 ; y -= 0.1)
  {
    m = acos(y) * 10 ;
    for(x = 0 ; x < m ; x++)
      printf(" ");

    printf("*");

    /*  62 means 2 * pi * 10    */
    for(x = m + 1 ; x < 62 - m ; x++)
      printf(" ");

    printf("*\n");
  }
  
  return 0 ;
  //exit(0);

}
