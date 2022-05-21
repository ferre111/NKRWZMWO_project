cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER, gcc)
set(CMAKE_CXX_COMPILER, g++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_compile_options(-Wall -Wextra -pedantic -Werror)

project(tmp)

add_executable(${CMAKE_PROJECT_NAME} 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

target_link_libraries(${CMAKE_PROJECT_NAME}  m)

# Add header directories (AFTER add_executable!!!)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/inc)
        
