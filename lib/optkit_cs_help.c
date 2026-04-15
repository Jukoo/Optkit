//!SPDX-License-Identifier:GPL-3.0 

#include <assert.h> 
#include <errno.h> 
#include <stdarg.h> 

#include "optkit_macro.h"
#include "optkit_cs_help.h"  

//!NOTE::Warning: THIS ! is a shared resources between ios stream operation  
//!     : on memery buffer.

FILE * optkit_stream = 00; 

struct __optkit_iomsbuf {
  FILE  *_bmrec  ; 
  char  *_bptr ; 
  size_t _bytes; 
} optkit_iorecorder; 

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
     .close = iomem_close
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
         loffset  = 0  , 
         bwriten = 0 ; 
  unsigned int partition_idx = new_ctx_cookies->_partition_index ;
  char *buffer_expension = 00 ; 

  /* 
   * Expand the  page  buffer  when the global offset reach the end of buffer.
   * It'll doubling the size of the page allocation.
   * TODO::FEAT: 
   * Should make a copy of that page buffer and expand each section by ?(percent%) 
   * */ 
  if((global_offset &~(mbios->_page_allocation-1))){
    if(!__expand_buffer(new_ctx_cookies))   
      return -ENOMEM; 
  }

  waddr_offt = __get_partition_location_address(new_ctx_cookies , _IOM_WRITE);   

  loffset =  write_at(waddr_offt , user_buffer , wbytes) ;
  if(-1 == loffset) 
    return  -EOVERFLOW ;  

  bwriten = (loffset >> 0x10) ; 
  loffset = loffset  & 0xffff ;  
 
  /*Keep  saving the real size of the buffer*/ 
  if (bwriten) 
    new_ctx_cookies->_real_buffer_size+=bwriten+2;  
  
  /* Update the local offset of the current partion index. 
   * NOTE : each section or partition has single local cursor 
   * offset called loffset or local offset used for checkpoint. 
   */
  new_ctx_cookies->_scope_interval[partition_idx]._coffset= loffset ;   


  /* Update the global offset  
   * NOTE:The current allocated page buffer.
   * has a global cursor used to check if the 
   * page buffer reach the end.
   * always take the value to highest local offset. 
   */
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

int iomem_close(void * ctx_cookies) 
{
  struct __optkit_memsb_t * global_ctx_cookies = (struct __optkit_memsb_t*) ctx_cookies ;
  if(global_ctx_cookies->_msbio_cookies) 
  {
    struct __membuff_cookies_t * page_buffer = global_ctx_cookies->_msbio_cookies; 
    if(page_buffer->_sbuff)  
      free(page_buffer->_sbuff), page_buffer->_sbuff =00 ; 

    free(page_buffer), page_buffer=00; 
  }
  return 0; 
}


static struct __optkit_memsb_t  * 
__update_context_cookies(struct __optkit_memsb_t * ctxc , int partition_index ,  int iom_flags)   
{
  ctxc->_partition_index = partition_index;  

  /* NOTE: In read context, the cursor offset is not saved.
   * Cauz we want to read the entire related block at once  
   */
  if(iom_flags & _IOM_READ){
    struct __section_t p = ctxc->_scope_interval[partition_index] ; 

    p._coffset = 0 ;  
    memcpy((ctxc->_scope_interval+partition_index) , &p , sizeof(p)) ;  
  } 

  /* !NOTE: no operation  for _IOM_WRITE mode.
   * Because we need to remember where we were at the  related block buffer.
   * The cursor offset is automaticaly saved  by the <<iomem_write>>  hook.  
   */

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
  
  ctx_cookies =  *rupdate(&ctx_cookies , partition);
  
  breaded =fread((buffer+bcheck),1,breaded,optkit_stream) ; 
  
  if(bf_check) 
    bcheck+=breaded ; 

  return breaded;   
}

int optkit_iombufdestroy(void) 
{
  return fclose(optkit_stream) ;
}


size_t optkit_iombufsize(unsigned char **buffer)  
{
  size_t tbbs =  ctx_cookies._real_buffer_size ;
  *buffer = calloc(sizeof(char),tbbs);  
  if(!buffer) 
    return -1 ; 
   
  return tbbs ; 

}
