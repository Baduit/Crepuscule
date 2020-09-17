cmake_minimum_required(VERSION 3.12)

project(CrepusculeBuild VERSION 0.1 DESCRIPTION "An easy to use tokenizer")

add_subdirectory(lib)

add_executable(example ./bin/main.cpp)
target_compile_features(example PRIVATE cxx_std_20)
target_compile_options(example
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)

target_link_libraries(example PRIVATE crepuscule)
                      
include(CTest)
enable_testing(MAKE_DIRECTORY test)
add_subdirectory(test)