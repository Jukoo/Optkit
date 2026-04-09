//SPDX-License-Identifier: GPL-3.0 
//!--This is a test file --- 

#include <stdio.h> 
#include "optkit.h" 



extern char * optkit_pbn ;
extern char * optkit_help; 

optkit_begin(options, "This program is  a test\n\
just a illustration programm")  

noarg("help" ,  "show this help" ) ,  
rearg("output" ,"redirect output") ,
oparg("input" , "redirect input")  ,

  optkit_ends("The end footer should content the copyright\n\
,author and the version of the program") 

int main(int ac , char * const *av) 
{

  optkit_parse(options , av) ;  
  char * basename = optkit_get_basename(av) ;


  return 0 ; 
}
