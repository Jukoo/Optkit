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
  
  optkit_t   optk = {._optkit_base = options , ._xinfo_flags =0};  

  optkit_pbn = optkit_get_basename(av) ;
  mopt._max_entries  =  __optkit_get_entries(options) ;
  optkit_looking_extra_info(&optk)  ; 
  mopt._ds_goptl= optkit_extract_option(optk._optkit_base,  &mopt); 

  if(!mopt._ds_goptl){
     optkit_err(optkit_extract_option) ; 
     return ;  
  }
  optk._optkit_mcollect  = &mopt ; 
  
  
  optkit_dump(&optk) ; 
 
  //!__optkit_clean(mopt) ; 
  return  ; 
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
       optkit->_extrainfo[i]  = malloc(sizeof(struct __optkit_extra_info_t)) ; 
       if(!optkit->_extrainfo[i])
         continue ; 

       optkit->_extrainfo[i]  = (struct __optkit_extra_info_t *) xinfo_attr[i]._type; 
       optkit->_xinfo_flags+=(i+1)  ;  
     } 
     
  } 
   
}

void optkit_show_usage(void) 
{ 
  setvbuf(stdout ,  00 , _IONBF , 0);  
  printf("%s", helper_record._bptr); 
 
}



   
