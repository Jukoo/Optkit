
AM_CFLAGS= -Wall -Wextra  -Wpedantic  -O0 -g -std=gnu11 -I bits/types 

bin_PROGRAMS=optkit
optkit_SOURCES = optkit.c test.c 

.PHONY:  clean  clean-local  mproper 

clean-local: 
	@rm Makefile
	@rm Makefile.in 

clean: 
	@rm  *.o  

mproper: clean-local clean 
	@rm $(bin_PROGRAMS)  

 

