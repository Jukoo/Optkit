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
     .close =  (void *) 00, //iomem_close
     .seek  =  (void *) 00 /*!NOT needed */ 
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
 
  //!Disable buffering on  optkit_stream ... 
  setvbuf(optkit_stream ,  00 , _IONBF ,0 ) ; 

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
 
  //Keep  saving the real size of the buffer; 
  if (bwriten) 
    new_ctx_cookies->_real_buffer_size+=bwriten+2;  
  
  //update the local offset of the current partion index  
  new_ctx_cookies->_scope_interval[partition_idx]._coffset= loffset ;   


  //!update the global offset  
  if(mbios->_offset <  loffset ){ 
    mbios->_offset = mbios->_checkpoint= loffset ; 
  }

  free(waddr_offt) , waddr_offt =0 ; 
  return   bwriten ;  
} 

ssize_t  iomem_read(void * ctx_cookies , char *   ubuff ,  size_t rbytes) 
{ 
 
  struct __optkit_memsb_t *  new_ctx_cookies = (struct __optkit_memsb_t*)  ctx_cookies ; 
  struct __section_t *secpart = (new_ctx_cookies->_scope_interval+new_ctx_cookies->_partition_index); 
  struct __page_io_location_t * raddr_offt = 00; 
  size_t rb = 0 ;  

  raddr_offt =  __get_partition_location_address(new_ctx_cookies , _IOM_READ);   
  rb = read_at(raddr_offt , ubuff ,  rbytes);  

  memcpy(secpart,  &raddr_offt->_chunck_part , sizeof(struct __section_t)) ; 
  free(raddr_offt) ,raddr_offt=00; 

  //!NOTE : The cursor offset  is not updated  !WARNING! 
   
  return rb  ; 
}

static struct __optkit_memsb_t  * 
__update_context_cookies(struct __optkit_memsb_t * ctxc , int partition_index ,  int iom_flags)   
{
  ctxc->_partition_index = partition_index;  
  
  if(iom_flags & _IOM_READ){
    //in read mode ;
    struct __section_t p = ctxc->_scope_interval[partition_index] ; 
    
    //!NO saved: because the read mode read  on block at once ; 
    p._coffset = 0 ;  
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
   char *s = 0 ;
   //!Update the  global  cookies context stream ... 
   ctx_cookies = *wupdate(&ctx_cookies , partition) ; 
   vasprintf(&s, fmt , ap) ; 
   va_end(ap) ;  
  
   bwriten = fwrite(s,1,strlen(s),optkit_stream) ; 
  
   return bwriten ; 
}

size_t optkit_rat(int partition , char *buffer, int bf_check)  
{
  struct __section_t section = ctx_cookies._scope_interval[partition];  
  size_t breaded =  section._coffset;  
  static unsigned int bcheck =0 ;  
  //!Update the  global  cookies context stream ... 
  ctx_cookies =  *rupdate(&ctx_cookies , partition);
  
  breaded =fread((buffer+bcheck),1,breaded,optkit_stream) ; 
  //memset((buffer+breaded) , 0xa  , 2 ) ; 
  //breaded+=2 ; 
  if(bf_check) 
    bcheck+=breaded ; 
 

  return breaded;   
}


size_t optkit_iombufsize(unsigned char **buffer)  
{
  size_t tbbs =  ctx_cookies._real_buffer_size ;
  *buffer = calloc(sizeof(char),tbbs);  
  if(!buffer) 
    return -1 ; 
   
  return tbbs ; 

}

