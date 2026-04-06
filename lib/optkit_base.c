/*SPDX-License-Identifier: GPL-3.0 */ 


#define _GNU_SOURCE  
#include "optkit_base.h" 
#include <stdio.h> 
#include <string.h>  
#include <errno.h>  

char * optkit_pbn =(char *) 00 ; 
optkit_meta_t mopt = {0} ; 
optkit_recbuf_t  helper_record= {0} ; 
optkit_xtra_info_t _s , _f ;



char * optkit_get_basename(char *const * argv) 
{
   int jmp_idx  = 0 ; 
   jmp_idx ^=HAS_RPATH_SYMB(*argv) ; 
    
  return optkit_copy_bn(*argv ,  jmp_idx) ; 
} 


void optkit_parse(base_optkit_t *restrict options, char * const  *av)   
{
  
  if (!init_memstream_buffer_cookies()) 
  {
    fprintf(stderr , "fail to initialize memory stream buffer \012") ; 
    return ; 
  }
  
  optkit_pbn = optkit_get_basename(av) ;
  mopt._max_entries  =  __optkit_get_entries(options) ;
  optkit_looking_extra_info(options) ; 
  mopt._ds_goptl= optkit_extract_option(options ,  &mopt); 

  if(!mopt._ds_goptl){
     optkit_err(optkit_extract_option) ; 
     return ;  
  }
  //optkit_show_usage() ; 
  
  optkit_dump() ; 
 
  //!__optkit_clean(mopt) ; 
  return  ; 
} 

int optkit_looking_extra_info(base_optkit_t  * options) 
{ 
  size_t t =  strlen(_s._xinfo) +  strlen(_f._xinfo) ; 

  if(0 < t) ; 
  {
     options->_extrainfo[0] = malloc(sizeof(struct __optkit_extra_info_t)) ; 
     if(!options->_extrainfo[0]){
      return ~0 ; 
     }
     options->_extrainfo[0] =  &_s ; 
  }
  


}

void optkit_show_usage(void) 
{ 
  setvbuf(stdout ,  00 , _IONBF , 0);  
  printf("%s", helper_record._bptr); 
 
}



   
