#  -*- Optkit Meson build  -*- 
#  -*- Author: Umar Ba <jUmarB@protonmail.com> 

project(
  'Optkit',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)

cc = meson.get_compiler('c')  

add_project_arguments( 
  cc.get_supported_arguments(
    '-Wformat',
    ), 
  language:'c'
  ) 

subdir('lib') 

executable( 
  'test_optkit', 
  'test.c',
  include_directories:['lib'],
  link_with:optkit_so
  )


