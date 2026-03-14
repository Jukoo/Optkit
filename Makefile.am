AUTOMAKE_OPTIONS = foreign 

SUBDIRS=lib 

bin_PROGRAMS=test 
test_SOURCES=test.c 
test_LDADD=lib/liboptkit.la 
test_CFLAGS=-I lib 


clean-local:
	@rm config.status config.log
	@rm Makefile  
	@rm -r autom4te.cache/
	@rm aclocal.m4
	@rm compile install-sh missing Makefile.in
	@rm stamp-h1  
	@rm *~  config.guess config.sub depcomp 


