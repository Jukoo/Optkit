#SPDX-License-Identifier : GPL-3.0 

AUTOMAKE_OPTIONS=foreign 

lib_LTLIBRARIES = liboptkit.la

liboptkit_la_SOURCES = lib/optkit_base.c \
					   lib/optkit_cs_help.c\
					   lib/bits/types/opkit_cs_help.h\
					   lib/bits/types/optkit_t.h

liboptkit_la_LDFLAGS = -version-info 1:0:0

include_HEADERS = lib/optkit.h \
				  lib/optkit_base.h\
				  lib/optkit_macro.h

noinst_PROGRAMS = optkit_example
optkit_example_SOURCES = test.c
optkit_example_LDADD = liboptkit.la
