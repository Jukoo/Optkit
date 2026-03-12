#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#include "arghlp.h" 

#define  HEADER "This is just a demo ...\n\
  to see how to use it"

#define FOOTER "\
This is simply the help footer like What you see in regular POSIX command line e.g:\n\
GNU coreutils online help: <https://www.gnu.org/software/coreutils/>\n\
Full documentation <https://www.gnu.org/software/coreutils/cat>\n\
or available locally via: info '(coreutils) cat invocation'"


struct optionx optlx[] ={ 
  {{"help", no_argument , nullable, 'h'}      , "\t\tprint help of the program"}, 
  {{"version",no_argument,nullable, 'v'}      , "\t\tprint the version of the program"},
  {{"input", required_argument,nullable, 'i'} , "<file>\tinput of the program"},
  {{"output",optional_argument,nullable, 'o'} , "[file]\toutput of the program"},
  OPTX_END 
}; 



void  * argparse(int ac  ,char * const * av , const char *shortopt ,  struct option * opts ,  void * args) 
{
  int opt = 0 ; 
  if (1 == ac ) __help__ ; 

  while( (opt  = getopt_long(ac , av,  shortopt ,  opts ,  0)) != ~0 ) 
  {
    switch(opt) 
    {
      case  'h': 
        __help__;
        break; 
      case 'v':  
        puts("version 1.x.x");  
        break; 
      case 'i':
        char *input = optarg ; 
        fprintf(stdout, "input -> %s\n" , input) ; 
        break; 
      case 'o': 
    
        char *output = optentry( av , ac , optind) ; 
        fprintf(stdout, "output -> %s\n" , output) ; 
        break;

      default: 
        fprintf(stderr , "no  allowed ...\n");  
        break ;
    }
  }

  return (void *) args ;
  
}



int main (int ac , char **av)
{
  struct arghlp arghlp = {
    .synopsys   =  {HEADER , FOOTER} ,  
    .options    = optlx, 
    .ah_handler = argparse
  } ; 
  
  arghlp_context(ac , av ,  &arghlp ,  nullable) ; 
  

  return EXIT_SUCCESS ; 
}
