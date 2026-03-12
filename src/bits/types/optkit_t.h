/*SPDX-License-Identifier: GPL-3.0 */ 

#if !defined(_SYS_TYPES_OPTKIT_T_H) 
#define _SYS_TYPES_OPTKIT_T_H  1 

#if defined(__has_feature) 
# if !__has_feature(nullability) 
#   define _Nonnull  
#   define _Nullable 
# endif 
#else  
# define _Nonnull
# define _Nullable 
#endif 


#define OPTKIT_USAGE_PAGE_BUFFER_SIZE 4096 

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
typedef FILE * optkit_mstream ; 

typedef struct  __optkit_meta_t {
  gopt_t * _ds_goptl;  
  char *_shortopts ; 
  unsigned int  _max_entries ; 
}optkit_meta_t; 


typedef  struct __optkit_recbuf_t  {
   char  *_bptr ; 
   size_t _bytes; 
   optkit_mstream _bmrec  ; 
} optkit_recbuf_t; 

typedef char optkit_usage_page_buffer_t; 
typedef  optkit_usage_page_buffer_t  uhelpbuf_t[OPTKIT_USAGE_PAGE_BUFFER_SIZE] ; 

//!NOTE : define usage vector table  
enum{ 
  USAGE_SECTION= 0x000, 
#define  USAGE_SECTION      0x000   /* 0     ->  0xff  */ 
  SYNOPSYS_SECTION=0x100, 
#define  SYNOPSYS_SECTION   0x100   /* 0x100 ->  0x3ff */ 
  HELPER_SECTION=0x400, 
#define  HELPER_SECTION     0x400   /* 0x400 ->  0xbff */
  FOOTER_SECTION=0xc00 
#define  FOOTER_SECTION     0xc00   /* 0xc00 ->  0x1000*/  
};   

#endif 
