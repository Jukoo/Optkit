#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([optkit],[0.1],[jUmarB@protonmail.com])
AM_INIT_AUTOMAKE
LT_INIT([dlopen])
AC_CONFIG_SRCDIR([lib/optkit.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB 
AC_PROG_AR 
AC_PROG_CXX
AC_PROG_CPP 

# Allow to use POSIX standard facilities 
AC_SEARCH_LIBS([strerror],[cposix]) 

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strchr
                strrchr
                asprintf
                opem_memstream])

AC_CONFIG_FILES([Makefile
                 lib/Makefile])
AC_OUTPUT
