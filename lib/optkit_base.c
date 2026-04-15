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

static void __optkit_vcheck(unsigned int  noptions,  void * user_feed , va_list ap ) 
{
  
  struct local{ 
    optkit_parser_routine_cb  _cb_routine; 
    void * _data ; 
    union {
       void * _should_be_unref; 
    }; 
  } *user =  (struct  local *) user_feed ;  
  
#define  REFCHECK(pdata) \
  if(pdata == user->_should_be_unref) pdata=(void*)0  

  unsigned int idx=~0; 
  while((++idx)<noptions){
    switch(idx)
    {
        case 0 : 
          *(void **) & user->_cb_routine= va_arg(ap , optkit_parser_routine_cb) ;
          REFCHECK(user->_cb_routine) ; 
          break ; 
        case 1 : 
          user->_data= va_arg(ap, void *) ;  
          REFCHECK(user->_data) ; 
          break ; 
    }
  }

}
int optkit_parse(char  * const * av ,  base_optkit_t *restrict options , ...) 
{
  optkit_parser_routine_cb user_cb_routine = (void  *)0;  
  void *user_data =  (void *)0 ; 
  struct { 
     optkit_parser_routine_cb  _user_cb_routine; 
     void * _user_data ; 
     union {
       void * _should_be_unref; 
     }; 
  } feed = {  
    ._should_be_unref = (void *) options,  
  } ; 

  unsigned const int noptions=2;  
  va_list ap ; 
  va_start(ap,noptions)  ; 
  __optkit_vcheck(noptions, (void *) &feed, ap);
  va_end(ap) ; 


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
  optkit_handler_argument(optn ,feed._user_cb_routine , feed._user_data)  ; 

  optkit_clean(&optk) ; 

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

static int  optkit_clean(struct  __optkit_t * _Nonnull restrict   optkit) 
{
  gopt_t * getopt_options = optkit->_optkit_mcollect->_ds_goptl ; 
  char * shortoptions= optkit->_optkit_mcollect->_shortopts ; 
  if (getopt_options) 
    free(getopt_options), getopt_options=00 ; 
  
  if(shortoptions) 
    free(shortoptions), shortoptions=00; 

  optkit_close_cookie_stream_buffer(); 
  return 0;  
}

void  optkit_release_extern_object(void) 
{
   free(optkit_pbn), optkit_pbn=00; 
   free(optkit_help), optkit_help=00; 
  
   puts("free"); 
}
