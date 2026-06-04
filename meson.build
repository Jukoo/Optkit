#  -*- Optkit Meson build  -*- 
#  -*- Author: Umar Ba <jUmarB@protonmail.com> 

project(
  'Optkit',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['c_std=gnu99' , 'warning_level=3'],
)

cc = meson.get_compiler('c')  

add_project_arguments( 
  cc.get_supported_arguments(
  '-Wformat=2',
  '-Werror=format-security',
  '-Wnull-dereference',
  '-Wstrict-prototypes',
  '-Wmissing-prototypes',
  '-Wpointer-arith',
  '-Wshadow',
  '-Wswitch-default',
  '-Wunused-result',
  '-Wunused-function',
  '-Wunused-variable',
  '-Wpedantic',
  '-Wimplicit-fallthrough', 
  '-Wcalloc-transposed-args'
  ), 
  language:'c'
  ) 

subdir('lib') 

executable( 
  'test_optkit', 
  'test.c',
  include_directories:['lib' , 'lib/bits/types'],
  link_with:optkit_so
  )


