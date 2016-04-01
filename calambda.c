
/* 2016.03.16 pm 18:41 , Starbucks in CBD Fortune mall . 
   To play to try Gary's lambda ca.c .
   The program is from < The Computational Beauty of Nature > (by Gary Flake) .
*/

/* NAME
 *   ca - simulate arbitrary one-dimensional cellular automata
 * NOTES
 *   The main portion of this code is relatively straight forward,
 *   whereas the code to compute lambda (or random rules of a
 *   specified lambda) is a bit tricky and is solved by a method
 *   that resembles dynamic programming.
 *   
 *   Also, the states are stored in a circular buffer such that
 *   the sums for every cell are computed as fast as possible with
 *   the number of additions on the order of two per cell.
 * MISCELLANY
 *   When supplying a lambda value for a random rule, it may not
 *   be possible to find a string with that lambda value because
 *   one may not exist.  In this case, the program will do its
 *   best to find one as close as possible.  In any event, the
 *   algorithm for finding random rules strings for a specified
 *   lambda value is non-deterministic and may not always find
 *   a perfect match even if one exists.  However, it will work
 *   well with high probability, and even when it doesn't find
 *   a perfect match it almost always gets close.
 * BUGS
 *   No sanity checks are performed to make sure that any of the
 *   options make sense.
 * AUTHOR
 *   Copyright (c) 1997, Gary William Flake.
 *   
 *   Permission granted for any use according to the standard GNU
 *   ``copyleft'' agreement provided that the author's comments are
 *   neither modified nor removed.  No warranty is given or implied.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int height = 60 , width = 60 , states = 2 , radius = 3 ;
int wrap = 1 ;
char *rules = "00000110" ;
double lambda = -1.0 ;

/* show table by cols and rows */
void showtable(int **table , int area , int len)
{
  int a , b ;

  for(a = 0 ; a < area ; a++)
  {
    for(b = 0 ; b < len ; b++)
    {
      //printf("[%d][%d] = %d " , a , b , table[a][b]);
      printf("%d " , table[a][b]);
    }
    printf("\n");
  }
  printf("\n");
}

void dorules()
{
  int a , b , e , f , k , m , i ;
  int area , len , states , N ;
  int sum ;
  int **table , *bits ;
  double *vals ;

  /* 2d table for funny to try */

  area = 7 , len = 5 ;
  states = 4 ;
  N = (int)pow(states , area) ;
  
  table = malloc(sizeof(int *) * area);
  for(a = 0 ; a < area ; a++)
  {
    table[a] = malloc(sizeof(int) * len);
    /* old way to assign values */
    /*
    for(b = 0 ; b < len ; b++)
    {
      table[a][b] = ((a + b) > 2) ? 1 : 0 ;
    }
    */
  }

  /* assign values to line 1 */
  
  for(b = 0 ; b < len ; b++)
  {
    table[0][b] = (b < states) ? 1 : 0 ;
    //printf("b = %d , table value = %d\n" , b , table[0][b]);
  }  

  showtable(table , area , len);

  /* from line 2 on , assign values */
  for(e = 1 ; e < area ; e++)
  {
    for(f = 0 ; f < len ; f++)
    {
      sum = 0 ;
      for(k = 0 ; k < states ; k++)
      {
        if(f >= k) sum += table[e-1][f-k];
      }
      table[e][f] = sum;
    }
  }

  showtable(table , area , len);

  vals = malloc(sizeof(double) * len);
  for(i = 0 ; i < len ; i++)
  {
    /* if not convert N to double type ,
       vals will be int type , 
       so that it will be 0.000 .
     */
    vals[i] = table[area-1][i] / (double)N ;
    printf("vals[%d] =  %f\n", i , vals[i]);
  }

  bits = malloc(sizeof(int) * len);
  for(i = 0 ; i < len ; i++)
  {
    bits[i] = random() % 2 ;
    printf("bits[%d] = %d\n" , i , bits[i]);
  }
  
  /* clean up table . */
  for(m = 0 ; m < area ; m++)
  {
    free(table[m]);
  }
  free(table);

}

void cellsshow(int *cells , int leng , int leftline , int rightline)
{
  int i ;

  for(i = 0 ; i < leng ; i++)
  {
    if((i == leftline) || (i == rightline))
      printf("|");

    if(cells[i] < 10)
      printf("0%d " , cells[i]);
    else
      printf("%d " , cells[i]);
  }
  printf("\n");
}

void plotcells(char *cells , int leng)
{
  int a ;
  //printf("s2.1");
  for(a = 0 ; a < leng ; a++)
  {
    if(cells[a] == 0) printf(" ");
    else printf("*");
  }
  printf("\n");
  //printf("s2.2");
}

/* This function has two distinct modes of operation; thus, there are
   two ways of calling it.  The first way is with rules equal to a valid
   rules string (and, therefore, non-NULL).  In this case, the passed 
   lambda value is ignored and the rules string is analyzed to determine
   its lambda value.

   In the second mode, with rule equal to NULL, a random string is
   generated with a lambda value as close to the requested value
   as possible.

   In either case, both the states, radius, and sq arguments are used. */ 

char *doingrules(int states , int radius , double *lambda , char *rules)
{
  int area = 2 * radius + 1 ;
  int len = (states - 1) * area + 1 ;
  int N = (int)(pow(states , area)) ;
  int **table , *bits ;
  int a , b , aval , bval , noimprove ;
  int i , j , k , sum ;
  double *vals , oldlambda , newlambda ;

  /* Build table that contains the number of transitions to a particular
   * state.  More specifically, table[i][j] will contain the number of
   * possible ways in which a string of length (i + 1) can sum up to j
   * using any number for a state from 0 to (states - 1).  Thus, the
   * final row of table[area - 1] will contain the number of ways we
   * can sum up to the column for this area size.
   *
   * We compute this by exploiting the recurrence relationship
   *     table[i][j] = sum(k = 0 .. states - 1) table[i-1][j-k]
   * with any nonsense pair of [i-1][j-k] yielding zero.
   */

  /* build ini table . 
     the table means a all-possible-states table .
   */
  table = malloc(sizeof(int *) * area);
  for(i = 0 ; i < area ; i++)
    table[i] = malloc(sizeof(int) * len);    
  /* Start with base condition. */
  /* assign values for line 1 */
  for(i = 0 ; i < len ; i++)
    table[0][i] = (i < states) ? 1 : 0 ;

  /* Fill via recurrence relationship. */
  /* assign values by Gary's interesting way . */
  for(i = 1 ; i < area ; i++)
  {
    for(j = 0 ; j < len ; j++)
    {
      sum = 0 ;
      for(k = 0 ; k < states ; k++)
        if(j >= k)
          sum += table[i-1][j-k] ;
      table[i][j] = sum ;
    }
  }

  /* Now turn the final row of the table into lambda units. */
  /* turn the final column , then divide by N , 
     then put the results into vals[] .
     Why use the final column ? 
   */
  vals = malloc(sizeof(double) * len) ;
  for(i = 0 ; i < len ; i++){
    vals[i] = (double)( table[area - 1][i] / (double)N ) ;
    printf("vals = %f\n" , vals[i]);
  }

  /* Make a random vector of bits. */
  /* build bits[] . note length of bits[] equals length of vals[] .
     note that the bits[] values is just 0/1 .
   */
  bits = malloc(sizeof(int) * len);
  for(i = 0 ; i < len ; i++){
    bits[i] = random() % 2 ;
    printf("bits = %d\n" , bits[i]);
  }

  /* Calculate lambda for these bits.   Note that we are only
   * distinguishing quiescent from non-quiescent states at this point.
   */
  /* oldlambda value : 
     filter some vals[] values by using bits[] 0 value (bits[] , 0/1) .
     summary then put to oldlambda . 
   */
  oldlambda = 0 ;
  for(i = 0 ; i < len ; i++)
    oldlambda += vals[i] * bits[i];

  printf("oldlambda = %f\n" , oldlambda);
  printf("len = %d\n" , len);

  /* Now, try to improve the lambda count for a bunch of steps. */
  noimprove = 0 ;
  while(1)
  {
    /* by me : set "random%len" only for random%2 , 
       so two random setting -- " first wide random then tiny random " ,
       all of this is a 
       " wide-folwing integrated(flow through) tiny flowing " 
       - amazing flowing (-- Now , what flowing is it exactly ,
       I can't describe this exacly . )
    */

    /* Pick two random bits to change and two new values */
    /* setting-key-point A */
    while(1)
    {
      a = random() % len ;
      printf("a = %d\n" , a);
      /* ? */
      if(a % area == 0) continue ;
      aval = random() % 2 ;
      break;
    }

    /* setting-key-point B */
    while(1)
    {
      b = random() % len ;
      printf("b = %d\n" , b);
      if(b % area == 0) continue ;
      bval = random() % 2 ;
      break;
    }

    /* aval(bval) just 0/1 , bits[] just 0/1 .
       so , aval(bval) - bits[] just three values -- 0/1/-1 .
       so , (xxx above) * vals[] just three types -- 0/+vals[]/-vals[] .
       so , newlambda is a adjusting-result 
       with many vals array-object values .

       If so , the using final column of the varing-table to make vals[] 
       existed a significance . 
       So , what is the significance ? Maybe it is relate to a lambda rules .
       About - - the final column of the varing-table ... 
       ???

     */
    printf("s0.01 = %f\n" , (aval - bits[a]) * vals[a]);
    printf("s0.02 = %f\n" , (bval - bits[b]) * vals[b]);
    newlambda = oldlambda + (aval - bits[a]) * vals[a] +
      (bval - bits[b]) * vals[b] ;

    printf("newlambda = %f\n" , newlambda);

    /* If this is better, then accept it. */
    /* so , ~lambda is a vals-adjusting .
       so , the diff (between ~lambda and *lambda(I guess it is a standard)) 
       is a distance . To find a min distance , do it again and again .

       FOR finding the closest xxx-way (away from *lambda) , to match 
       many bits[] values . All of this is for training the bits[] to 
       more meet *lambda-rules .

     */
    if(fabs(newlambda - *lambda) < fabs(oldlambda - *lambda))
    {
      bits[a] = aval ;
      bits[b] = bval ;
      oldlambda = newlambda ;
      noimprove = 0 ;
    }
    else
      noimprove++ ;

    /* We've gone a long time without improving things, so quit. */
    /* try 1000 times to find the min distance . */
    //if(noimprove == 1000) break ;
    if(noimprove == 10) break ;

  }
  /* output bits */
  for(i = 0 ; i < len ; i++)
  {
    printf("bits = %d\n" , bits[i]);
  }

  /* Now that we have the bits, we can pick state transitions
   * and return the new rule. 
   */
  /* why (len + 1) ? maybe because last pos of string is set to 0 . */
  rules = malloc(sizeof(char) * (len + 1)) ;
  rules[len] = 0 ;
  /* I guess that bits[] just is 0/1 (because aval amd bval just are 0/1 .) , 
     so "random%(states-1)" means that makes all possible(random) 
     state values for the ca object .
   */
  for(i = 0 ; i < len ; i++)
  {
    rules[i] = bits[i] ? ((random() % (states - 1)) + '1') : '0' ;
  }

  printf("rules==%s\n" , rules);

  for(i = 0 ; i < area ; i++)
    free(table[i]) ;
  free(table) ;
  free(bits) ;
  free(vals) ;

  return rules;
}

int main(int argc , char **argv)
{
  char *old , *new , *swap ;
  int i , a ;
  int sum , odd = 2 ;

  //dorules();

  /* The extra 2 * radius + 2 cells are "buffers" that allow us to
   * compute the sums rapidly.
   */
  old = malloc(sizeof(char) * (width + 2 * radius + 2));
  new = malloc(sizeof(char) * (width + 2 * radius + 2));
  /* width + 2r + 2 , equals to (r+1) + width + (r+1) 
     for expanding a length of (radius+1) each side
     based on width . amazing ! -- by me
  */

  /* assign values */
  for(i = 0 ; i < (width + 2 * radius + 2) ; i++)
  {
    old[i] = new[i] = 0 ;
  }

  /* Initialize the first state . */
  for(i = (radius + 1) ; i < (width + radius + 1) ; i++)
  {
    if(random() % odd == 0)
      old[i] = random() % (states - 1) + 1 ;
  }
  //old[width / 2] = 1 ;
  //cellsshow(old , (width + 2*radius + 2) , 3 , 19);

  plotcells(old , (width + 2 * radius + 2));
  //printf("s05\n");
  rules = doingrules(states , radius , &lambda , rules) ;

  /* The main loop.  All of the real work is done here. */
  for(a = 0 ; a < height ; a++)
  {
    /* assign by Gary */
    /*
    for(i = 0 ; i < radius ; i++)
    {
      old[i + 1] = old[width + 1 + i] ;
      old[width + radius + 1 + i] = old[radius + 1 + i];
    }
    */
    //cellsshow(old , (width + 2*radius + 2) , 3 , 19);
    /*
    for(i = 0 ; i < (width + 2 * radius + 2) ; i++)
    {
      old[i] = i ;
    }
    */
    //cellsshow(old , (width + 2*radius + 2) , 3 , 19);

    /* assign by me . the left edge connects to the right edge . */
    for(i = 0 ; i < radius ; i++)
    {
      /* put the left values of the right edge values
	 on the buffer to the left . -- by me */
      //printf("{old[%d]==%d}<=" , i , old[i]);
      //printf("{old[%d]==%d}\n" , (width+i+1) , old[width+i+1]);
      old[i] = old[width + i + 1] ;

      /* put the right values of the left edge values
	 on the buffer to the right . -- by me */
      //printf("{old[%d]==%d}<=" , (width+radius+i+2) , old[width+radius+i+2]);
      //printf("{old[%d]==%d}\n" , (radius+i+1) , old[radius+i+1]);
      old[width + radius + i + 2] = old[radius + i + 1];
    }

    //cellsshow(old , (width + 2*radius + 2) , 3 , 19);

    /* initialize the sum . */
    for(i = 0 , sum = 0 ; i < 2 * radius + 1 ; i++)
    {
      sum += old[i];
    }

    /* If we know the sum of the cell to the left then we just subtract
     * its left-most neighbor within radius and add our right-most
     * neighbor within radius to compute this cell's sum.
     */
    for(i = (radius + 1) ; i < (width + radius + 1) ; i++)
    {
      sum = sum + old[i+radius] - old[i-radius-1] ;
      new[i] = rules[sum] - '0' ;
    }

    plotcells(new , (width + 2 * radius + 2));

    swap = old , old = new , new = swap ;

  } /* for(a = 0 ; a < height ; a++) */

  printf("hello\n");
  exit(1);
}
