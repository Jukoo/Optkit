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

typedef struct __base_optkit_t {
   struct option _lopt; 
   const char *  _description; 
} base_optkit_t ; 

typedef  struct  __optkit_t { 
   struct  __base_optkit_t * _optkit_base ;
   char  * _sysnosys  ; 
   char  * _footer ; 
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


typedef  struct __optkit_recbuf_t  {
  FILE * _bmrec  ; 
  char  *_bptr ; 
  size_t _bytes; 
} optkit_recbuf_t; 

#endif 
