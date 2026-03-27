//!SPDX-License-Identifier: GPL-3.0  
/* 
 * - Optkit helper cookiestream  
 *   A Custum memory stream io 
 *   for helper or usage . 
 */

#if !defined(_SYS_TYPE_OPTKIT_CS_HLP)  
#define _SYS_TYPE_OPTKIT_CS_HLP 

#if !defined(_GNU_SOURCE)  
# define _GNU_SOURCE   
#endif 
#define _FILE_OFFSET_BITS  64  

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#if defined(UNISTD_H)  
# define HELPER_STREAM_BUFFER_SIZE  sysconf(_SC_PAGESIZE) 
#else   
# if !defined(HELPER_STREAM_BUFFER_SIZE)  
#   define HELPER_STREAM_BUFFER_SIZE   0x1000
# endif 
#endif 

//!NOTE : define helper usage as vector table  
enum{ 
  USAGE_SECTION= 0, 
#define  _USAGE_SECTION    {0x000,0xff}  
  SYNOPSYS_SECTION, 
#define  _SYNOPSYS_SECTION {0x100,0x3ff} 
  HELPER_SECTION, 
#define  _HELPER_SECTION   {0x400,0xbff}   
  FOOTER_SECTION, 
#define  _FOOTER_SECTION   {0xc00,0x1000}  
}; 

#define NSECTIONS 4  

typedef char iocookie_stream_buf_t; 
typedef  iocookie_stream_buf_t   iocbs_t [HELPER_STREAM_BUFFER_SIZE] ; 


typedef struct __section_t   section_t ; 
typedef struct __membuff_cookies_t membuff_cookies_t ;  
typedef struct __optkit_memsb_t  optkit_memsb_t ; 

struct __section_t { 
  size_t  _begin, 
          _end; 
  union  { 
    off_t _coffset ;   //used to remember  where were  you last time    
  } ; 
};   


struct __membuff_cookies_t {  
  char *   _sbuff ;  
  void *   _ebuff ;  

  size_t   _endpos , 
           _page_allocation;  

  off_t    _offset,       //used  for rewind  
           _checkpoint;   //used  for persistant checkpoint 
  
}; 

typedef struct __section_t sections_t[NSECTIONS];   

struct __optkit_memsb_t {    
  struct __membuff_cookies_t *_msbio_cookies ;  
  sections_t  _scope_interval; 
  unsigned char  _partition_index ; 
}; 

struct  __page_io_location_t{ 
   void * _io_location_address ;  
   struct __section_t _chunck_part; 
};  


extern   FILE *  optkit_stream ; 
extern   struct __optkit_memsb_t ctx_cookies ;   


static  inline struct  __membuff_cookies_t* 
__setup_membuff_cookies(void) 
{
  struct __membuff_cookies_t * mcookies = malloc(sizeof(*mcookies)) ; 
  if(!mcookies) 
    goto _free_mcookies ;

  
  mcookies->_sbuff = malloc(HELPER_STREAM_BUFFER_SIZE) ; 
  if(!mcookies->_sbuff) 
    goto _free_mcookies ; 


  mcookies->_page_allocation = HELPER_STREAM_BUFFER_SIZE;  
  
  mcookies->_ebuff = (mcookies->_sbuff+HELPER_STREAM_BUFFER_SIZE) ;  
  mcookies->_endpos =  mcookies->_offset = 0 ; 

  return mcookies ; 


_free_mcookies: 
  free(mcookies) , mcookies=00 ; 

  return 00 ;  
}
static inline struct __page_io_location_t  * 
__get_partition_location_address(struct __optkit_memsb_t * new_ctxc) 
{
  unsigned start = 0 ; 
  struct  __page_io_location_t * ioloc = malloc(sizeof(struct  __page_io_location_t *)) ;  
  if(!ioloc) 
    return 00 ; 
  
  struct  __section_t  secpart =  new_ctxc->_scope_interval[new_ctxc->_partition_index]; 

  ioloc->_io_location_address =  new_ctxc->_msbio_cookies->_sbuff+secpart._begin ;   
  
  memcpy(&ioloc->_chunck_part , &secpart ,   sizeof(struct __section_t)) ; 

  
  //!CHECK OVERFLOW ...
  if(ioloc->_io_location_address > new_ctxc->_msbio_cookies->_ebuff)
  {
    free(ioloc) ,  ioloc = 00 ;  
    return (void* ) -10  ; // overflow code  
  }

  return ioloc ;  

}

static inline  size_t  
write_at(struct __page_io_location_t * ioloc , const char * ubuff ,  size_t  wbytes) 
{
  ioloc->_chunck_part._coffset+=wbytes;  

  if(ioloc->_chunck_part._coffset >= ioloc->_chunck_part._end) 
    return  EOF ;  


  memcpy(ioloc->_io_location_address,ubuff , wbytes) ; 
  return ioloc->_chunck_part._coffset ;  
}


static inline void *  __expand_buffer(struct __optkit_memsb_t * new_ctxc) 
{
  char * buffer_expension  = 00  ; 
  struct __membuff_cookies_t *  mbios = new_ctxc->_msbio_cookies; 

  mbios->_page_allocation<<=1 ;  
  buffer_expension = realloc(mbios->_sbuff , mbios->_page_allocation); 
  if(!buffer_expension) 
    return   00 ; 
  
  mbios->_sbuff = buffer_expension ; 
  mbios->_ebuff = (mbios->_sbuff+ mbios->_page_allocation) ; 

  return  new_ctxc ; 

}
struct  __optkit_memsb_t *  
init_memstream_buffer_cookies(void) ;  



/* Predefined Hooks  for cookies stream   io manipulation */ 
ssize_t  
iomem_write(void * ctx_cookies , const char * buff , size_t wbytesize) ; 

ssize_t  
iomem_read(void * ctx_cookies , char * buff  , size_t rbytesize) ;  

int 
iomem_seek(void * ctx_cookies ,  off_t *offset , int whence) ; 

int 
iomem_close(void * ctx_cookies) ; 


static struct __optkit_memsb_t  *  
__update_context_cookies(struct __optkit_memsb_t  * ctx_cookies,  int partindex ,  int iomode_flags) ; 

static  struct __optkit_memsb_t *
wupdate(struct __optkit_memsb_t * ctx_cookies,  int partindex); 
static  struct __optkit_memsb_t *
rupdate(struct __optkit_memsb_t * ctx_cookies,  int partindex); 

size_t 
optkit_wat(int partion  , const char * fmt , ...) ; 


#endif //!   _SYS_TYPE_OPTKIT_CS_HLP 
