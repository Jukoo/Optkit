/** @file  arghlp.h  
 *  @brief extend getopt for a more readable use of 
 *         arguments with an intergrated description
 *         field for improved code readability,
 *         Define your helper at the same time as you
 *         define your flags 
 *
 *  @Copyright (C) 2024 Umar Ba jUmarB@protonmail.com  @OpenWire Studio .HomeLab
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

#if!defined  (__argument_helper)  
#define __argument_helper  

#if __GNUC_PREREQ(3,4) 
# pragma once 
#endif 

#include <getopt.h>

#ifdef  __cplusplus  
# define ARGHLP  extern "C"  
#else 
# define ARGHLP 
#endif 

#define  nullable NULL 

#define  MAX_BUFF   255

#ifndef  USAGE_BUFF
#define  USAGE_BUFF 5000 //! That's arbitrary 
#endif 

#define  OPT_END  { nullable,0,nullable,0} 
#define  OPTX_END {OPT_END,0} 

#define  OPT_END_DEF   (struct option) OPT_END 
#define  OPTX_END_DEF  (struct optionx) OPTX_END 

#define  def(__option) __option##_DEF

#ifndef BUILD_FROM_SNAME
//! By default  it  use option.val to build  short arguments 
# define BUILD_FROM_SVAL
#endif 

#define ARGENTRY_SYMB 58  

//!  FORMATING 
#if !defined(HELPER_FRMT)
# define HELPER_FRMT  " -%c, --%s %s" 
#endif

#if !defined(USAGE_FRMT) 
# define USAGE_FRMT  "USAGE: %s \n"
#endif 

//! __help__  
#define __help__ fprintf(stdout ,"%s\n", __usage)
#define __usage__  __help__
#define __display_usage__ __help__

enum { 
  REQUIRED,
#ifdef  required_argument  
# define REQUIRED  required_argument 
#endif

  OPTIONAL 
#ifdef optional_argument 
# define OPTIONAL optional_argument 
#endif 
}; 

enum { 
   ARGREQUIERED ,   
#define ARGREQUIERED  (1 << ARGREQUIERED)
   ARGOPTIONAL 
#define ARGOPTIONAL   (1 << ARGOPTIONAL) 
}; 

#define __attribute_option(__attr__) ARG#__attr__ 


typedef struct __flags_t flags_t   ; //! Opaque type of flags
extern char    __usage[USAGE_BUFF] ; //! 
extern char    __shopt[MAX_BUFF] ; 
extern struct  option  __optl[MAX_BUFF] ; 
extern int     __nargp ;            //! arguments counter  
                    

struct  optionx { 
  struct option optlong ; 
  char   optdesc[MAX_BUFF]; 
}; 

struct synopsys_t { 
  char * header_description ;
  char * footer_description ;  
}; 

typedef struct arghlp  arghlp;  

typedef  void *(*arghlp_handler)(int __ac , char *const * __av , 
    const char    *  __shortopt , 
    struct option *  __longoption  , 
    void * __wur __args) ;   


struct arghlp { 
  struct synopsys_t synopsys ; 
  struct optionx * options; 
  arghlp_handler  ah_handler;   
};  
 

ARGHLP static char  * get_program_basename(char *const  * __argument_vector) __wur __nonnull((1)); 
ARGHLP static void  * set_option( char  * __shortopt ,   int __has_arg ) ; 
ARGHLP static  char * build_short_option(const struct optionx * __raw_optionx) __wur __nonnull((1)); 
ARGHLP void static build_usage_helper(char* flags_t, const char * __flag_description) __nonnull((1,2)); 
ARGHLP static char * make_sysnopsys(char *__basename, struct arghlp * __synopsys, char * __usage_body) __nonnull((1,2,3)) ; 
ARGHLP static struct option * extract_getopt_option(struct  option *  __g_option) __nonnull(()) ;
ARGHLP void  *arghlp_context ( int __argcounter, char * const *__argvector , 
                               const  struct arghlp *__arghlp , void * __wur __argxtra) __nonnull((2,3));

ARGHLP char *optentry(char * const * __av ,int __ac  , int __optindex) ;  

#endif //!__argument_helper  
