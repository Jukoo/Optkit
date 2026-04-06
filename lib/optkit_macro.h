//!SPDX-License-Identifier: GPL-3.0 

#if !defined(_M_optkit) 
#define  _M_optkit 

#include <stdio.h> 
#include <stddef.h>
#include <stdarg.h> 

#define  HAS_RPATH_SYMB(__pn)  !(0x2e2f ^ ((*__pn)<<8|(*__pn+1))) 
#define __sterr__  "%s" , strerror(*__errno_location())  

#define optkit_err(f) \
  do{perror(#f), fprintf(stderr , __sterr__);}while(0) 

#define  USGFMT   " Usage : %s :%s\012\012"
#define  HLPFMT   "  -%c, --%s \011\011%s\012" 

#define optkit_write_at(offset_region , ... ) \
  sprintf(offset_region , __VA_ARGS__) 

#define  OPTKIT_USAGE_CLASSIFICATIONS  {\
    "[OPTION]... [ARGS]...",\
    "[OPTION]...",\
    "[ARGS]...",\
} 
#define  MAX_BUF 0xFF 


#define optkit_sysnopsys(...) \
  optkit_write_at(synosys ,  __VA_ARGS__)


#define  optkitnull {{00 ,0 ,00,0}, 00}  
#define  __shopt(__loption) (__loption[0] & 0xff)  


#define optkit_begin(identifier , ... ) \
  optkit_xtra_info_t _s = {._xinfo =  __VA_ARGS__ };\
  base_optkit_t identifier[] = { 

#define noarg(__loption ,  __description) \
    {{__loption ,0 ,00, __shopt(__loption)} , __description}

#define  rearg(__loption , __description) \
    {{__loption ,1 ,00 , __shopt(__loption) } , __description} 

#define  oparg(__loption , __description) \
    {{__loption ,2 ,00 , __shopt(__loption)} , __description} 
  
#define  optkit_end  optkitnull }; 

#define optkit_ends(...) \
    optkitnull };\
    optkit_xtra_info_t _f = {._xinfo = __VA_ARGS__}; 

static inline const char * fva_args(const  char * fmt, ...) 
{
  va_list ap ; 
  va_start(ap,fmt) ; 
  char *s  = 0 ; 
  vasprintf(&s, fmt , ap) ;   

  va_end(ap);  
  return s ; 
}


#endif//_M_optkit
