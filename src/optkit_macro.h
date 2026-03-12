//!SPDX-License-Identifier: GPL-3.0 

#if !defined(_M_optkit) 
#define  _M_optkit 


#define  HAS_RPATH_SYMB(__pn)  !(0x2e2f ^ ((*__pn)<<8|(*__pn+1))) 
#define __sterr__  "%s" , strerror(*__errno_location())  

#define optkit_err(f) \
  do{perror(#f), fprintf(stderr , __sterr__);}while(0) 

#define  HLPFMT  "  -%c, --%s \011\011%s\012" 

#define optkit_write_at(offset_region , ... ) \
  sprintf(usage_page_buffer+offset_region , __VA_ARGS__) 

#define  OPTKIT_USAGE_CLASSIFICATIONS  {\
    "[OPTION]... [ARGS]...",\
    "[OPTION]...",\
    "[ARGS]...",\
} 


#define  optkitnull {{00 ,0 ,00,0}, 00}  
#define  __shopt(__loption) (__loption[0] & 0xff)  

/*NOTE: Suggestion 
 * #define optkit_begin(identifer , sysnopys , footer) 
 **/
#define optkit_begin(identier) \
  base_optkit_t identier[] = { 

#define noarg(__loption ,  __description) \
    {{__loption ,0 ,00, __shopt(__loption)} , __description}

#define  rearg(__loption , __description) \
    {{__loption ,1 ,00 , __shopt(__loption) } , __description} 

#define  oparg(__loption , __description) \
    {{__loption ,2 ,00 , __shopt(__loption)} , __description} 
  

#define  optkit_end  optkitnull } ;  
#endif//_M_optkit
