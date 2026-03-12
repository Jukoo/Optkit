//@file  elfcheck.c  
//@brief  check elf signature  on binnary file 
//@author Umar Ba  <jUmarB@protonmail.com>   <github/Jukoo>

#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <errno.h>
#include <string.h> 


#include "elfcheck.h" 

char bFullpath[255]={0}; 

static int  __has_execute_permition(const char * binfile) 
{
   (void *)__search(binfile) ; 
   if(~0==(access(bFullpath,X_OK)))   
     return errno ; 

   return 0 ;  
}

static char  *  __search(const char  * binfile) 
{
  char * system_path = getenv("PATH") ;  
  if (!system_path)  
    return NULL  ; 
  
  char * token = NULL ;  
  while( (token  =  strtok(system_path  , ":")) != NULL )  
  {
    sprintf(bFullpath, "%s/%s", token ,  binfile) ; 
    
    if (0 == (access(bFullpath,  F_OK)))  break  ; 

    explicit_bzero(bFullpath,  255) ; 

    if (system_path != NULL)  
      system_path =NULL ; 
    
  }

  return bFullpath;  
}

elf_sig_t elf_check(const char * binfile) 
{
  
   if ( __has_execute_permition(binfile) != 0  ) 
     return  ~0 ; 

   return  elf_signature_check(bFullpath) ;  

}

elf_sig_t elf_signature_check(const char *binfile)
{
  int  elfbin_fd = open(binfile , O_RDONLY) ;  
  if (~0 == elfbin_fd)
    return  ~0 ;

  //!TODO : charger le binnaire dans le memoire partager  
  //!       au cas  ou j'aurai besoin de faire une copie 
   
  char  elfchunk[ELFSIG_CHUNK] = {0} ; 
  size_t  requested_bytes =  read(elfbin_fd , elfchunk , ELFSIG_CHUNK);
  if (ELFSIG_CHUNK !=  requested_bytes) 
  {
    close(elfbin_fd) ; 
    return  0; 
  }

  int i =0 ; 
  char start_sig_found = 0 ; 
  elf_sig_t region_offset_size = ELF_SIG_BIT_SIZE_MAX; 
  elf_sig_t elfsignature_status = 1;  
  elf_sig_t mask_bit = 0 ; 
  elf_sig_t mask_offset_check=0; 
  while( i < ELFSIG_CHUNK )  
  {
    if(  *(elfchunk+i) == (__get_mask(__st)) &&  i == 0  &&  start_sig_found ==0)  
    {
      start_sig_found^=1 ;   
      //!jump to executable region 
      region_offset_size >>=  start_sig_found ;  
      mask_bit = __get_mask(__st) ; 
      i++; 
      continue ;  
    } 

    if(0 ==  start_sig_found)  return  ~0; 

    
    mask_bit= mask_bit << 8 | *(elfchunk+i) ; 
    mask_offset_check =  (ELF_SIG >> region_offset_size) ; 

    if (mask_offset_check == mask_bit) {
      elfsignature_status =(elfsignature_status <<1) | 1 ; 
      region_offset_size-=8 ;  
    }
    else 
      return ~0;  

    
    i++;
  }
 
  
  close(elfbin_fd);

  return   (__get_mask(__st))^ elfsignature_status ;
  
}
