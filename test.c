//SPDX-License-Identifier: GPL-3.0 
//!--This is a test file --- 

/* TODO: 
 * []  - add builtin argparse  that handle only the helper ... 
 */ 
#include <stdio.h> 
#include "optkit.h" 

//extern char * optkit_pbn ;
extern char * optkit_help; 

optkit_begin(options, "This program is  a test\n\
just a illustration programm")  

noarg("help" ,  "show this help" , 'h') ,  
rearg("output" ,"redirect output",  'O') ,
oparg("input" , "redirect input")  ,

optkit_ends("The end footer should content the copyright\n\
,author and the version of the program") 

void x(int * i  ,  void * args ) 
{
  
  switch(*i) 
  {
    case 'O': 
      puts("the big O notation") ; 
      break ; 
  }


}
int main(int ac , char * const *av) 
{
  int z = 10 ; 
  
  if(optkit_parse(av , options ,x))  
    fprintf(stderr , "%s" , optkit_help) ;
 
  

  return 0 ; 
}
