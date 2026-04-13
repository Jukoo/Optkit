/*SPDX-License-Identifier: GPL-3.0 */ 

#if !defined(_SYS_TYPES_OPTKIT_T_H) 
#define _SYS_TYPES_OPTKIT_T_H  1 

#include <stddef.h> 
#include <getopt.h>
#include <stdio.h> 

#if defined(__has_feature) 
# if !__has_feature(nullability) 
#   define _Nonnull  
#   define _Nullable 
# endif 
#else  
# define _Nonnull
# define _Nullable 
#endif 


typedef typeof(const char * (const char *fmt , ...)) *fxtract  ; 

typedef  struct __optkit_extra_info_t  optkit_xtra_info_t; 
struct __optkit_extra_info_t { 
  fxtract _f ;  //NOTE  : not used  yet  !  
  char * _xinfo ;  
} ; 

typedef struct __base_optkit_t {
   struct option _lopt; 
   const char *  _description;
   union {
    unsigned char _sopt_override;
   }; 
} base_optkit_t ; 

typedef  struct  __optkit_t { 
   struct  __base_optkit_t * _optkit_base ;
   struct  { 
     struct __optkit_extra_info_t * _extrainfo[2] ;
     unsigned char _xinfo_flags; 
   }; 
   char *const * _argv;  
   union { 
     struct __optkit_meta_t *_optkit_mcollect ; 
   };   

} optkit_t ;  
 
typedef struct option  gopt_t ; 

typedef struct  __optkit_meta_t {
  gopt_t * _ds_goptl;  
  char *_shortopts ; 
  unsigned int  _max_entries ; 
}optkit_meta_t; 


//* Opacque  declaration use in internal */ 
struct __optkit_iomsbuf ;    


#endif 
