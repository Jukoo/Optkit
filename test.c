//SPDX-License-Identifier: GPL-3.0 
//!--This is a test file --- 

#include <stdio.h> 
#include "optkit.h" 



extern char * optkit_pbn ;

optkit_begin(options, "The synopsys  lore")  
  noarg("help" ,  "show this help" ) ,  
  rearg("output" ,"redirect output") ,
  oparg("input" , "redirect input")  ,
optkit_ends("The end footer")   

int main(int ac , char * const *av) 
{

  printf("%s \012" , _s._xinfo) ; 
  optkit_parse(options , av) ;  
  char * basename = optkit_get_basename(av) ;

  printf(" %s \012" , basename)  ; 


  return 0 ; 
}
