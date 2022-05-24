cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER, /usr/bin/gcc)
set(CMAKE_CXX_COMPILER, /usr/bin/g++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_compile_options(-Wall -Wextra -pedantic -Werror)

project(project)

add_executable(${CMAKE_PROJECT_NAME} 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
                ${CMAKE_CURRENT_SOURCE_DIR}/src/file_read.c
                ${CMAKE_CURRENT_SOURCE_DIR}/src/dfs.c)

target_link_libraries(${CMAKE_PROJECT_NAME}  m)

# Add header directories (AFTER add_executable!!!)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/inc)
        
