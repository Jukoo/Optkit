#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT(optkit,1.0, Umar Ba <jUmarB@protonmail.com> ) 
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([test.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_AR
AC_PROG_CC
#+ Looking for libtool 
LT_INIT 

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_OFF_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([fopencookie], [] ,
    [
    AC_MSG_ERROR("fopencookie : missing. The optkit required this fonctionalty to proceed ")
    ]
) 
AX_CFLAGS_WARN_ALL
CFLAGS="-I lib  -I lib/bits/types $CFLAGS -Wformat=2 \-Werror=format-security -Wnull-dereference -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Wshadow -Wswitch-default -Wunused-result -Wimplicit-fallthrough"


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
