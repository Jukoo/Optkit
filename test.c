//SPDX-License-Identifier: GPL-3.0 
/*--This is a test file And you can refer to this as example ---  
 *  This file  answer the Question how to use it ?? 
 * Copyright(c) 2026 , Umar Ba <jUmarB@protonmail.com> 
 */ 

#include <stdio.h> 
#include "optkit.h" 

extern char * optkit_help; 

optkit_begin(options, "This program is  a test\n\
just a illustration programm")  

noarg("help" ,  "show this help" , 'h') ,  
rearg("output" ,"redirect output",  'O') ,
oparg("input" , "redirect input")  ,

optkit_ends("The end footer should content the copyright\n\
,author and the version of the program") 

void argument_handler(int * option ,  void *  your_extra_data) 
{
  
  if(your_extra_data)  
  {
     puts("has something inside") ; 
  }

  switch(*option) 
  {
    case 'O': 
      puts("the big O notation") ; 
      break ; 
    case 'i':  
      printf("Input  data   %s \n" ,  optarg) ; 
      break; 
    default:
      printf("%s" , optkit_help) ; 
      break ; 
  }


}
int main(int ac , char * const *av) 
{
  int  your_extra_data= 10 ; 
  
  if(optkit_parse(av , options , argument_handler,  (void *) &your_extra_data))  
    fprintf(stderr , "%s" , optkit_help) ;
 
  

  return 0 ; 
}
