#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <err.h>  

#include "arghlp.h" 
#include "elfcheck.h" 

#define  SYNOPSYS "[FILE]...\n\
  Check the Given file has ELF signature e.g \n" 

#define  FOOTER "GNU coreutils online help: <https://www.gnu.org/software/coreutils/>\n\
available locally via: <no ressources>" 


struct  optionx  opt[]= {
  {{"help"   ,0  , NULL, 'h'},  "\t\tPrint This help"},
  {{"version",0  , NULL, 'v'},  "\t\tShow the version of the program"}, 
  {{"elf"    ,1  , NULL, 'E'},  "[bFILE]\t Check file is an elf format"},
  OPTX_END 
}; 

void * argument_parser(int ac , char * const *av , const char  * shortopt  , struct option * optl  , void * args) 
{
   if (1 == ac ){
     __help__ ; 
     exit(EXIT_SUCCESS) ; 
   }
   int option_argument = 0 ; 

   while ( ~0  != (option_argument = getopt_long(ac , av ,  shortopt,  optl, 0)) )
   {
     switch(option_argument) 
     {
       case 'h' : 
         __help__; 
         exit(EXIT_SUCCESS) ; 
         break; 
       case 'v':
         fprintf(stdout  , "version %s \n" , ELFSIG_VERSION) ;
         exit(EXIT_SUCCESS) ; 
         break; 
      case  'E': 
         if (NULL != args )  
           memcpy(args  , optarg ,  strlen(optarg)) ; 
         break ; 
      default :  
         fprintf(stderr , "Argument  not Allowed ...\n") ; 
         break ; 
     }
   }
  
   return  args ; 
}

int main(int ac , char **av)
{ 

  struct arghlp  argh =  {   
    .synopsys = {SYNOPSYS,FOOTER},  
    .options= opt, 
    .ah_handler= argument_parser
  }; 
  
  char execfile[0x14] ={0} ; 
  arghlp_context(ac , av , &argh,  execfile ) ;  
 
  if ( 0 ==  strlen(execfile)){
    errx(~0 , "Require File") ;  
  }

  elf_sig_t  elfbin_status = elf_check(execfile) ; 

  if (~0 == elfbin_status){
    errx(EXIT_FAILURE , "Error: Not an ELF file - it has the wrong magic bytes at the start"); 
  }  

  
  if (elfbin_status & ELF_SIG_COMPLETE)  
    fprintf(stdout , "ELF File - has the rigth magic bytes at the start\n") ; 


  return EXIT_SUCCESS; 
}
