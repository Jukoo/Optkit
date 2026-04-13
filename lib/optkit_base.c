/*SPDX-License-Identifier: GPL-3.0 */ 


#define _GNU_SOURCE  
#include "optkit_base.h" 
#include <stdio.h> 
#include <string.h>  
#include <errno.h>  

char * optkit_pbn =(char *) 00 ; 
char * optkit_help =  00;  

optkit_meta_t mopt = {0} ; 
optkit_xtra_info_t _s , _f; 
optkit_t optk ; 



static char * optkit_set_basename(char *const * argv) 
{
   int jmp_idx  = 0 ; 
   jmp_idx ^=HAS_RPATH_SYMB(*argv) ; 
    
  return optkit_copy_bn(*argv ,  jmp_idx) ; 
} 

//!TODO  : 
//int optkit_parse(char  * const * av ,  base_optkit_t *restrict options , ...) ; 
int optkit_parse(char * const * av  , base_optkit_t * restrict  options , optkit_parser_routine_cb _user_agrhld_fb,  void * args)  
{
 
  if(!init_memstream_buffer_cookies())
  {
    errno = EOPNOTSUPP; 
    optkit_err(init_memstream_buffer_cookies) ; 
    return errno ;  
  }
  
  optk._optkit_base = options ; 
  optk._xinfo_flags =0;
  optk._argv = av ; 
  int optn = 0 ; 

  optkit_pbn = optkit_set_basename(av) ;
  mopt._max_entries  =  __optkit_get_entries(options) ;
  optkit_looking_extra_info(&optk)  ; 
  mopt._ds_goptl= optkit_extract_option(optk._optkit_base,  &mopt); 

  if(!mopt._ds_goptl){
     optkit_err(optkit_extract_option) ; 
     return -1 ;  
  }
  optk._optkit_mcollect  = &mopt ; 
  optkit_register()  ; 

  optkit_handler_argument(optn , _user_agrhld_fb , args )  ; 

  //!__optkit_clean(mopt) ; 
 
  return 0 ;  
} 

int optkit_looking_extra_info(struct __optkit_t *  optkit) 
{  
  struct { 
    void  * _type; 
    size_t  _size ; 
  } xinfo_attr[2] ={
    {(void *) &_s , strlen(_s._xinfo)},
    {(void *) &_f , strlen(_f._xinfo)} 
  }; 

  unsigned int i  = ~0 ; 
  
  while(++i < 2 ) 
  {  
    if(0 < xinfo_attr[i]._size){
      char * xinfo  =  ((struct __optkit_extra_info_t*)(xinfo_attr+i)->_type)->_xinfo ; 
      optkit_record_extra_info(xinfo, i+1) ;
    }
     
  }
  return 0 ; 
}

int optkit_handler_argument(int opthdl,  optkit_parser_routine_cb  handler, void * args)  
{
   unsigned int ac  =~0; 
   while(*(optk._argv+(++ac))); 

   while(~0!=(opthdl=getopt_long(ac , optk._argv , mopt._shortopts, mopt._ds_goptl, 0 ))) 
     !handler ? __optkit_default_builtin_arghandler(&opthdl,(void *)00):handler(&opthdl, args) ; 

   return ac ; 
}


void  __optkit_default_builtin_arghandler(int *option_handler ,  void *  __unused_ds)  
{
   switch(*option_handler)
   {
     case 'h': 
     default: 
       fprintf(stderr , "%s", optkit_help) ;  
       break ; 
   }

}

