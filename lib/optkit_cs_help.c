//!SPDX-License-Identifier:GPL-3.0 

#include "optkit_cs_help.h"  
#include <errno.h> 
#include <stdarg.h> 




//!NOTE::Warning: THIS ! is a shared resources between ios stream operation  
//!     : on memery buffer.

FILE * optkit_stream  = (FILE *) 00 ;  
struct __optkit_memsb_t ctx_cookies  = {00 , {
    _USAGE_SECTION, 
    _SYNOPSYS_SECTION, 
    _HELPER_SECTION,
    _FOOTER_SECTION
}, 
  USAGE_SECTION}; 
  
cookie_io_functions_t  hooks = { 
     .write = iomem_write, 
     .read  = iomem_read, 
     .close =  (void *) 00, 
     .seek  =  (void *) 00
     /*.read= iomem_read, 
      *.close=iomem_close,
      *.seek =iomem_seek
      */
  }; 


//init_memstream_buffer_cookies(int user_lock_permission) 
struct __optkit_memsb_t * 
init_memstream_buffer_cookies(void) 
{
  ctx_cookies._msbio_cookies=  __setup_membuff_cookies() ; 
  if(!ctx_cookies._msbio_cookies) 
    return 00 ;  

  optkit_stream  =  fopencookie(&ctx_cookies , "w+" ,  hooks) ; 
  
  if(!optkit_stream) 
  {
     free(ctx_cookies._msbio_cookies), ctx_cookies._msbio_cookies=00; 
     return  00 ; 
  }
  
  setvbuf(optkit_stream ,  00 ,  _IONBF ,0 ) ; 

  return  &ctx_cookies ; 
} 

ssize_t
iomem_write(void * ctx_cookies , const char * user_buffer , size_t wbytes) 
{ 

  struct __optkit_memsb_t *new_ctx_cookies =(struct  __optkit_memsb_t *) ctx_cookies ;
  struct __membuff_cookies_t  * mbios =  new_ctx_cookies->_msbio_cookies ;  
  struct __page_io_location_t * waddr_offt = 00 ;  
  
  size_t global_offset =   mbios->_offset +  wbytes , 
         local_offset  = 0  ; 
  unsigned int partition_idx = new_ctx_cookies->_partition_index ;
  char *buffer_expension = 00 ; 
  
  /* 
   * Expand the  page  buffer  when the global offset reach 
   * the end of buffer. by doubling the size of _page_allocation 
   * */ 
  if((global_offset &~(mbios->_page_allocation-1))){
    if(!__expand_buffer(new_ctx_cookies)) 
      return -ENOMEM; 
  }

  waddr_offt = __get_partition_location_address(new_ctx_cookies , _IOM_WRITE);   

  local_offset =  write_at(waddr_offt , user_buffer , wbytes) ;
  if(-1 == local_offset) 
    return  -EOVERFLOW ;  

  size_t bwriten = (local_offset >> 0x10)  , 
         loffset = (local_offset &  0xffff) ;  
  
  //update the local offset of the current partion index  
  //new_ctx_cookies->_scope_interval[partition_idx]._coffset+=local_offset ;  
  new_ctx_cookies->_scope_interval[partition_idx]._coffset= loffset ;   


  //!update the global offset  
  if(mbios->_offset <  loffset ){ 
    mbios->_offset = mbios->_checkpoint= loffset ; 
  }

  free(waddr_offt) , waddr_offt =0 ; 
  return  wbytes ; 
} 

ssize_t  iomem_read(void * ctx_cookies , char *   ubuff ,  size_t rbytes) 
{
  static int i = 0; 
  i++ ; 

  printf("times  %i\012" ,i) ; 
  struct __optkit_memsb_t *  new_ctx_cookies = (struct __optkit_memsb_t*)  ctx_cookies ; 
  struct __section_t secpart = new_ctx_cookies->_scope_interval[new_ctx_cookies->_partition_index]; 
  struct __page_io_location_t * raddr_offt = 00; 
  size_t rb = 0 ;  

  
  raddr_offt =  __get_partition_location_address(new_ctx_cookies , _IOM_READ); 
  
  printf(" %s  ::  %s \012", __func__ ,  (char *) raddr_offt->_io_location_address) ;  
  rb = read_at(raddr_offt , ubuff ,  rbytes); 

  //!NOTE : The cursor offset  is not updated  !WARNING! 

  return rb  ; 
}

static struct __optkit_memsb_t  * 
__update_context_cookies(struct __optkit_memsb_t * ctxc , int partition_index ,  int iom_flags)   
{
  ctxc->_partition_index = partition_index;  
  
  if(0 < iom_flags){
    //in read mode ;
    struct __section_t p = ctxc->_scope_interval[partition_index] ;  
    //p._coffset =  p._begin ; 
    memcpy((ctxc->_scope_interval+partition_index) , &p , sizeof(p)) ;  
  } 

  return ctxc ; 
}

struct __optkit_memsb_t  * wupdate(struct __optkit_memsb_t * ctx  , int partition_idx) 
{
  return  __update_context_cookies(ctx , partition_idx , _IOM_WRITE) ; 
}
struct __optkit_memsb_t * rupdate(struct __optkit_memsb_t * ctx , int partition_idx ) 
{
  return  __update_context_cookies(ctx , partition_idx , _IOM_READ) ;  
}


size_t  optkit_wat(int  partition ,  const char * fmt  , ...)  
{
   size_t  bwriten=0 ; 
   va_list ap ; 
   va_start(ap , fmt); 
  
   //!Update the  global  cookies context stream ... 
   ctx_cookies = *wupdate(&ctx_cookies , partition) ;
  
   bwriten = vfprintf(optkit_stream , fmt , ap) ;  
   va_end(ap) ; 

   return bwriten ; 
}

size_t optkit_rat(int partition , char * ubuff ,   size_t rbytes) 
{
  size_t breaded =0 ;  

  //!Update the  global  cookies context stream ... 
  ctx_cookies =  *rupdate(&ctx_cookies , partition); 
  
  fgets(ubuff , rbytes , optkit_stream) ;  

  //breaded = fread(ubuff, rbytes,1,optkit_stream) ;

  return rbytes ; 
}
