/* @file  elfcheck.h
 * @brief just check the signature of  bin file is executable
 */

#ifndef  __elfcheck 
#define  __elfcheck

#define ELFSIG_VERSION "1.0"   
#define ELF_SIG   0x7f454c46 

//#define   0x23212f62 signature for script  like bash 

#define ELF_SIG_BIT_SIZE_MAX   0x20
#define ELFSIG_CHUNK (ELF_SIG_BIT_SIZE_MAX / 8)

#define ELF_SIG_OFFSET__st    0x18 
#define ELF_SIG_OFFSET__ex    0x10 
#define ELF_SIG_OFFSET__lk    0x8 
#define ELF_SIG_OFFSET__ft    0x0 

#define ELF_SIG_COMPLETE      0x70 

#define __get_mask(__section)  \
  (ELF_SIG >> ELF_SIG_OFFSET##__section) 



//! determine the architecture size  ( 32 or 64 ) 
#define __arch_size sizeof(void *) * (1<<3) 

extern char **environ;
extern char bFullpath[255] ;  
typedef  int  elf_sig_t ; 

enum __elf_partion_offset__ { 
  __st,  // start offset  
  __ex,  // executable offset 
  __lk,  // linkable offset 
  __ft   // format offset 
}; 


static int __has_execute_permition(const char *__binfile)__nonnull((1)) ; 
static char *__search(const char *__binfile)  __nonnull((1)) ; 

int  elf_check(const char * __binfile)__nonnull((1));  

elf_sig_t elf_signature_check(const char *__binfile)   __nonnull((1)); 

#endif 
