 /*  @file arghlp.c 
  * @Copyright (C) 2024 Umar Ba jUmarB@protonmail.com  @OpenWire Studio .HomeLab
  *         This program is free software: you can redistribute it and/or modify
  *         it under the terms of the GNU General Public License as published by
  *         the Free Software Foundation, either version 3 of the License, or
  *         (at your option) any later version.
  *        
  *         This program is distributed in the hope that it will be useful,
  *         but WITHOUT ANY WARRANTY; without even the implied warranty of
  *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *         GNU General Public License for more details.
  */

#include <stdlib.h> 
#include <unistd.h> 
#include <libgen.h>
#include <stdio.h> 
#include <string.h>
#include <ctype.h> 
#include  "arghlp.h"

char __usage[USAGE_BUFF] = {0} ; 
char __shopt[MAX_BUFF] ={0}; 
int __nargp = 0 ; 

struct option  __optl[MAX_BUFF]; 
struct __flags_t{ 
  char *long_flags ; 
  char short_flags ; 
};


static char * get_program_basename(char *const  * argvector)  
{
     return  __xpg_basename( *argvector)   ;  
}

static void *  set_option(char * restrict shortopt ,  int  has_arg)
{
  return  memset(shortopt , ARGENTRY_SYMB,  has_arg) ; 
}
static char * build_short_option(const  struct optionx * optxlist) 
{
  struct option * opt = nullable;
  struct optionx * optx = (struct  optionx *)  optxlist ;
  
  char * shortopts = calloc(MAX_BUFF ,sizeof(char*)) ;
  if (!shortopts) 
    return nullable ; 

  int  optindex = 0 ; 
  int  soptindex= 0 ; 
  while( ((optx+optindex))->optlong.name != nullable) 
  {
    opt = &(optx+optindex)->optlong;
    *(__optl+optindex)= *opt;

#ifdef BUILD_FROM_SNAME
    *(shortopts+soptindex)= opt->name[0] ;
#else 
    *(shortopts+soptindex)=  opt->val;
#endif 

    //! just used  to  easy build of  --longflags   -l 
    flags_t  flags =  { .long_flags=(char*)opt->name , .short_flags=*(shortopts+soptindex) }  ; 

    build_usage_helper((char *)&flags, (optxlist+optindex)->optdesc) ; 
    switch (opt->has_arg) 
    {
       case required_argument:  
         soptindex++; 
         set_option((shortopts+soptindex) , REQUIRED) ; 
         break;
       case optional_argument: 
         set_option((shortopts+ ++soptindex) ,OPTIONAL) ; 
         soptindex++; 
         break; 
       case no_argument:
         break; 
    }
    optindex++ ; 
    soptindex++; 
    explicit_bzero(opt , sizeof(*opt)) ; 
  }
  memcpy(__shopt , shortopts , strlen(shortopts)) ; 
  free(shortopts) ; 
  *(__optl+optindex)= def(OPT_END) ;
  __nargp =  optindex++ +1  ;

  return __shopt ; 
}

void static build_usage_helper(char * flags, const char * flag_description) 
{
  char s[MAX_BUFF]=  {0}; 
  flags_t  *f = (flags_t*) flags ; 
  
  sprintf(s , HELPER_FRMT,  f->short_flags ,  f->long_flags , flag_description) ; 
 
  if (!strrchr(s,0xa)) 
     strcat(s,"\n") ; 
  
  strcat(__usage ,s) ;   

}

static char * make_synopsys(char * bn , struct arghlp * ah_synopsys, char * usage_body) 
{
  if(!ah_synopsys) 
    return usage_body ; 
  

  char usage_tmp[USAGE_BUFF] =  {0} ; 
  //! Save the usage body  
  memcpy(usage_tmp , usage_body ,  strlen(usage_body)) ;   

  //!clean usage body 
    
  explicit_bzero(usage_body ,USAGE_BUFF) ; 
  struct  synopsys_t *s=  &ah_synopsys->synopsys ;  
  
  char *header = s->header_description ; 
  char *footer = s->footer_description ;  

  sprintf(usage_body , USAGE_FRMT , bn) ;   
  
  if (header!= nullable) {
    //!just replace the  '\n' if header  if not null for
    //! better readability 
    char * remove_sn = strrchr(usage_body , 0xa) ; 
    *remove_sn = 0 ; 
    
    sprintf((usage_body+strlen(usage_body)) , "%s \n\n", header) ; 
  }
  
  memcpy((usage_body+strlen(usage_body)), usage_tmp , strlen(usage_tmp)) ;
  
  if(footer != nullable)
    sprintf((usage_body+strlen(usage_body)) , "\n%s", footer) ; 
  
  return usage_body ; 
}

void *arghlp_context ( int ac    , char * const * av  ,  const struct  arghlp *  arghlp , void * ax ) 
{
  char * bn  = get_program_basename(av) ;
  char * sopt = build_short_option(arghlp->options); 
  struct arghlp   *synopsys =  ( struct  arghlp * ) arghlp ; 
  (void *) make_synopsys( bn , synopsys, __usage) ;

  struct option  getopt_option[__nargp];  
  
  if (!extract_getopt_option(getopt_option)) 
    return nullable ; 
  
  return arghlp->ah_handler(ac , av , sopt,  getopt_option , ax);  

}

static struct option * extract_getopt_option(struct  option * g_opt) 
{
  if(  0 >= __nargp ) 
    return nullable ; 

  int i = ~0 ; 
  while(++i < __nargp  ) 
     *(g_opt+i) =  *(__optl+i); 

  return g_opt ;  
}

char *optentry(char *const *av  , int ac  ,int optindex)   
{
  
  if(ac == optindex) 
    return   optarg  ; 
  
  char *optfname= (char *) *(av + (optindex -1)) ; 
 
  if(0x2d == (*optfname & 0xff) && 0x2d !=  (*(optfname +1) & 0xff ))
    return optarg ; 

  //! looking for  equal symbol  "=" 
  
  char *equ_symb= strchr(optfname ,  0x3d & 0xff ) ;  
  if (equ_symb)  
  {
    size_t symb_offset= equ_symb -  optfname  ;   
    char *next= equ_symb+1; 
    if (*next == '\0')   
    {
      goto __maybe_next; 
    }

    //! --option=argument
    return  strdup(next) ; 
  }
  goto __maybe_next; 

__maybe_next: 
  char *optfname_value = (char *) *(av+optindex);
  
  if(0x2d == (*optfname_value & 0xff)) return optarg ; 

  //!  --option =args 
  if(0x3d == (*optfname_value & 0xff)) 
  {
    optfname_value++ ; 
    if (0 == (*optfname_value & 0xff))  
    {
      optindex++ ; 
      goto __maybe_next ; 
    }
    return strdup(optfname_value)  ; 
  }

  //! --option= args
  return  strdup(optfname_value) ; 

}
