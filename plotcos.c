
/* 20151202 23:32 start . Plot cos-curve with printf() .  */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void plot_cos_curve()
{
  double y ;
  int x , m ;
  
  for(y = 1 ; y >= -1 ; y -= 0.1)
  {
    //modified 1 , 
    m = acos(y) * 10 ;
    //m = acos(y) * 5 ;

    for(x = 0 ; x < m ; x++)
      printf(" ");

    printf("*");

    /* 62 means 2 * pi * 10 .  */
    //modified 2 , 
    //for(x = m + 1 ; x < 31 - m ; x++)
    for(x = m + 1 ; x < 62 - m ; x++)
    {
      printf(" ");
    }

    printf("*\n");

  } /*  for(y = 1 ; y >= -1 ; y -= 0.1)  */
  
}

void plot_sin_curve()
{
  int m = 0 , x ;
  double y ;

  for(y = 1 ; y > 0 ; y -= 0.1)
  {
    m = asin(y) * 10 ;
    //printf("%d\n" , m);
    
    for(x = 0 ; x < m ; x++)
      printf(" ");
    printf("*");

    for(x = m + 1 ; x < 31 - m ; x++)
      printf(" ");
    printf("*");

    printf("\n");
    
  } /*  for(y = 1 ; y >= 0 ; y -= 0.1)  */

  /*  line when y == 0  */
  for(x = 0 ; x <= 62 ; x++) 
  {
    if(x== 0 || x == 31 || x == 32 || x == 62)
      printf("+");
    else
      printf("-");
  }
  printf("\n");

  m = 0 ;
  for(y = -0.1 ; y >= -1 ; y -= 0.1)
  {
    m = asin(y) * 10 ;
    //printf("%d\n" , m);
    
    for(x = 0 ; x < 31 + abs(m) ; x++)
      printf(" ");
    printf("*");

    for(x = 32 + abs(m) ; x < 62 - abs(m) ; x++)
      printf(" ");
    printf("*");

    printf("\n");
    
  } /*  for(y = 0 ; y >= -1 ; y -= 0.1)  */
}

void plot_sin_curve_2(int occupy , double spos , double epos , double interval)
{
  int m = 0 , x ;
  double y ;

  /* 15 11 9 ... -9 -11 -15 */
  //y = 1 ;   y > 0 ;  y -= 0.1
  //y = 0.1 ; y <= 1 ; y += 0.1

  y = spos ;
  while(1)
  {
    /*output spaces of pi-value*/
    for(x = 0 ; x < occupy ; x++)
      printf(" ");

    m = asin(y) * 10 ;

    for(x = 0 ; x < m ; x++)
      printf(" ");
    printf("*");

    for(x = m + 1 ; x < 31 - m ; x++)
      printf(" ");
    printf("*");

    printf("\n");

    y += interval ;

    if(occupy == 0) 
    {
      if(y <= epos) break ;
    }
    else 
    {
      if(y > epos) break ;
    }

  }

}

int main()
{
  int x ;

  //plot_cos_curve();

  //plot_sin_curve();

  /*  fun to sin , start  */
  plot_sin_curve_2(0 , 1.0 , 0.0 , -0.1);

  for(x = 0 ; x <= 62 ; x++) 
  {
    if(x== 0 || x == 31 || x == 32 || x == 62)
      printf("+");
    else
      printf("-");
  }
  printf("\n");

  plot_sin_curve_2(31 , 0.1 , 1.0 , 0.1);

  /*  fun to sin , end  */

  printf("Finish!\n");
  return(0);
}
