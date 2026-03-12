cmake_minimum_required(VERSION 3.25) 
project(arghlp.elf)
set(CMAKE_C_STANDARD 99) 
set(CMAKE_C_STANDARD_REQUIRED ON) 

set(CMAKE_BUILD_TYPE Debug)  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/bin) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/lib) 

add_library(arghlp SHARED  arghlp.c) 

add_executable(${PROJECT_NAME} main.c) 
target_link_libraries(${PROJECT_NAME}  arghlp)
