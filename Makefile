#   This is for those  who don't have meson build system ...  - 
#
CC=gcc 
ARFLAGS=ar  -cr 
CFLAGS= -Wall -Wextra -pedantic  -O0  -g  -I  lib/bits/types
LDFLAGS= -shared -fPIC  
SOURCES=$(wildcard  lib/*.c) 
OBJECTS=$(SOURCES:.c=.o)
BUILDIR=build
elfobf=liboptik.so 
arfobf=$(elfobf:.so=.a)
all:$(elfobf) 

build:  
	@mkdir $(BUILDIR) 

$(elfobf) : $(SOURCES) 
	$(warning  [LD]  $<)   
	@$(CC) $(LDFLAGS)  $<  -o  $(BUILDIR)/$@  $(CFLAGS)   


$(arfobf) : $(OBJECTS) 
	$(warning [AR]  $<) 
	@ar  -cr $(BUILDIR)/$@   $<  


%.o:%.c 
	$(info  [CC]  $<)   
	@$(CC)  -c $< -o   $@  $(CFLAGS) 


.PHONY:  clean  mproper 

clean :  
	$(info  cleaning object files) 
	@rm $(OBJECTS)  

mproper: clean 
	$(info  deep cleaning  ...) 
	@rm   $(elfobf) $(arfobf) 
