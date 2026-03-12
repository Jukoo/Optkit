cmake_minimum_required(VERSION 3.25) 
project(elfcheck  VERSION 1.0  DESCRIPTION "Check binnary file looking elf signature and more ...")

set(CMAKE_BUILD_TYPE Debug) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin) 
 
add_executable(${PROJECT_NAME} main.c  elfcheck.c)
target_link_libraries(${PROJECT_NAME}  arghlp)

target_link_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../lib) 

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
