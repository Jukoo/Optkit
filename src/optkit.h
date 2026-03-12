//SPDX-License-Identifier: GPL-3.0 

#if !defined(__optkit_h) 
#define __optkit_h 

#define _GNU_SOURCE
#include <stdio.h> 
#include <unistd.h> 
#include <getopt.h> 
#include <string.h> 
#include <stdlib.h> 
#include <string.h> 
#include "optkit_macro.h"
#include "bits/types/optkit_t.h"

//! just one record is enougth ...   
extern optkit_recbuf_t  helper_record ;        /*Record helper  in memory buffer*/ 
extern optkit_meta_t mopt ;  
extern char * optkit_pbn ;  
extern uhelpbuf_t usage_page_buffer ;  


//! Or use strlen  
#define chrlen(str) __get_amount_nsize(str , 0)  
static  inline size_t  __get_amount_nsize(const  char * basename , int index) 
{ 
  size_t size =  strlen(basename) ; 
   if(0 < index) 
     size-=index ; 
   
   return size;  
} 

static inline  char * optkit_copy_bn(const char * basename ,  int index) 
{
  ssize_t bytes_copy = 0 ; 
  bytes_copy = __get_amount_nsize(basename ,  index) ; 
  char *s  = (char *)00  , 
       *bn_addr = (char*)00 ; 

  optkit_pbn = calloc(sizeof(char) ,  bytes_copy); 
  if(!optkit_pbn)  
    return (void *)00 ; 

  index += (index > 0) ;
  s  = strrchr((basename+index) , '/');  
  if(s){
    index +=(s-(basename+index))+1; 
    bytes_copy-=index-1;
  }

  return memcpy(optkit_pbn ,(basename+index), bytes_copy); 
}

/* retrieve the number of flag  given and also the max size of 
 * need  for short options 
 * */
static inline size_t __optkit_get_entries(base_optkit_t * options) 
{
  size_t entry = ~0  , 
         shopt_len = 0;   

  while( (options+(++entry))->_lopt.name )
    shopt_len+=(options+(entry))->_lopt.has_arg;   

  return (entry<<8|shopt_len);  
}

static inline void __optkit_build_sopt(char *  _Nonnull  sopt , const  gopt_t  lopt) 
{ 
  
  static int idx = 0 ; 
  char *args =00 , *s = 00;  
  asprintf(&args , "%c?%is", *lopt.name, lopt.has_arg); 

  s  = strchr(args  , '?') ;  *s='%'; 
  sprintf((sopt+idx) ,  args,  (0 < lopt.has_arg ? ":" :"")); 

  s = strchr(sopt , 0x20); 
  if(s) 
    *s=':' ; 

  idx = strlen(sopt) ; 
  free(args) ; 

} 

static inline int __optkit_init_bmrec(void) 
{ 
  if(helper_record._bmrec) return 0; 
  helper_record._bmrec = open_memstream(&helper_record._bptr , &helper_record._bytes); 
  
  return  (helper_record._bmrec) ? 0 : ~0 ;  
  
} 


static inline  size_t  __optkit_breffing_usage(int  argreq) 
{ 
  char usage_brief[][0x64] = OPTKIT_USAGE_CLASSIFICATIONS ; 
  optkit_write_at(USAGE_SECTION,"Usage :%s %s",optkit_pbn , *(argreq + usage_brief)) ; 
  return 0 ;  
} 
/* Record options in memory stream buffer  */
static inline size_t __optkit_record_helper(base_optkit_t * option)  
{
  // __optkit_init_bmrec(); 
  //!TODO :  record usage: first : 
  static size_t argument_requested=3 ; 
  static int stat = 0; 
  if(argument_requested){
    argument_requested^= !(option->_lopt.has_arg^stat) ? option->_lopt.has_arg: stat; 
    if(stat != argument_requested)
      stat = argument_requested ; 
  }
  
  /* 
  fprintf(helper_record._bmrec , HLPFMT ,  
      option->_lopt.val ,option->_lopt.name, option->_description);   
  fflush(helper_record._bmrec) ;  

  
  return  helper_record._bytes  ; 
  */ 


  return argument_requested  ;  
  

}

static inline gopt_t  * optkit_extract_option(base_optkit_t*  options , 
    optkit_meta_t *__restrict__ meta_option)  
{
  int entries = 1 , shopt_size =entries;  

  entries+= (meta_option->_max_entries >> 8) ; 
  shopt_size = meta_option->_max_entries & 0xff ; 
  
  gopt_t * super_opt = (gopt_t*) 00 ; 
  super_opt = malloc(sizeof(gopt_t) * entries); 
  if(!super_opt) 
    return (void *) 00 ;  

  meta_option->_shortopts = calloc(sizeof(char) , shopt_size) ; 
  if(!meta_option->_shortopts) 
  {
    free(super_opt) ; 
    return (void *) 00 ; 
  } 

  unsigned int idx =~0 , argn; 
  while(++idx < entries){
    memcpy((super_opt+idx) ,&(options+idx)->_lopt, sizeof(gopt_t));  
    /*!collect and  build short options */ 
    base_optkit_t *  single_opt  = (options+idx) ; 
    if(!single_opt->_lopt.name)   
      continue ; 
    
    __optkit_build_sopt(meta_option->_shortopts, single_opt->_lopt) ;
    argn  =__optkit_record_helper(single_opt) ; 
  }
  
  //fclose(helper_record._bmrec); 
  
  __optkit_breffing_usage(argn) ; 
   

  return super_opt ;  
} 

extern char * optkit_get_basename(char *const *  argument_vector) ; 
extern void optkit_show_usage(void) ; 
extern void optkit_parse(base_optkit_t * __restrict__ _Nonnull options , char *const *av) ; 

#endif 
