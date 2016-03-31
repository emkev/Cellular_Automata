
/* 2016.02.23 pm 23:49 , start .
ca program from <nature> converts to C languge . just for fun .
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *cells , *nextgen ;
int num = 60 , steps = 100 ;
int ruleset[8] = {0 , 1 , 0 , 1 , 1 , 0 , 1 , 0} ;

char* itoaSp(int num)
{
  char *str = "" ;
  int i = 0 , numtemp = 0 , len = 0 ;
  
  numtemp = num ;
  do
  {
    numtemp = numtemp / 10 ;
    i++ ;
  }
  while(numtemp > 0);

  //printf("itoaSp , i = %d\n" , i);

  str = malloc(sizeof(char) * (i + 1)) ;
  numtemp = num ;
  len = i ;
  i = i - 1 ;
  //printf("s1\n");
  
  while(i >= 0)
  {
    str[i] = (numtemp % 10) + '0' ;
    numtemp = numtemp / 10 ;
    i-- ;
  }
  //printf("s2\n");
  str[len] = 0 ;
  //printf("itoaSp , str = %s\n" , str);
  
  return str ;
}

/* string convert to binary */
int atobin(char *str)
{
  int i = 0 , sum = 0 ;

  while(str[i])
  {
    sum = sum * 2 + (str[i] - '0') ;
    i++ ;
  }
  //printf("atobin , sum = %d\n" , sum);

  return sum ;
}


char* strjoin(char *str1 , char *str2)
{
  char *result = "" ;
  result = malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1)) ;
  
  //if(result == NULL)
  //  exit(1);

  strcpy(result , str1) ;
  //printf("strjoin , result1 = %s\n" , result);
  strcat(result , str2) ;
  //printf("strjoin , result2 = %s\n" , result);

  return result ;
}

int rules(int a , int b , int c)
{
  char *s ;

  s = strjoin(strjoin(itoaSp(a) , itoaSp(b)) , itoaSp(c)) ;
  //printf("rules , s = %s\n" , s);
  //s = itoaSp(a);

  int index = atobin(s) ;
  //printf("rules , index = %d\n" , index);

  return ruleset[index];
  //return 0;
}

void generate()
{
  int a ;
  int left , middle , right ;

  nextgen[0] = cells[0] ;
  nextgen[num - 1] = cells[num - 1] ;

  for(a = 1 ; a < num - 1 ; a++)
  {
    left = cells[a - 1];
    middle = cells[a];
    right = cells[a + 1];

    nextgen[a] = rules(left , middle , right);
  }

  cells = nextgen ;
}

int main(int argc , char **argv)
{
  int i , j , k ;
  int a = 100 ;
  int m ;

  cells = malloc(sizeof(int) * num);
  nextgen = malloc(sizeof(int) * num);

  for(i = 0 ; i < num ; i++)
  {
    cells[i] = 0 ;
  }
  cells[(num / 2) - 1] = 1 ;
  cells[num / 2] = 1 ;

  for(j = 0 ; j < steps ; j++)
  {
    //generate();
    for(k = 0 ; k < num ; k++)
    {
      if(cells[k] == 0) printf(" ");
      else printf("*");
    }
    printf("\n");

    generate();
  }

  //printf("main , s1");
  printf("hello\n");
  exit(0);
}
