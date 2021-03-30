cmake_minimum_required(VERSION 3.12)

project(CrepusculeBuild VERSION 1.1.0 DESCRIPTION "An easy to use tokenizer")

add_subdirectory(lib)

option(EXAMPLE "EXAMPLE" ON)
option(TESTS "TESTS" ON)

# Example
if (EXAMPLE)
    add_executable(example ./bin/main.cpp)
    target_compile_features(example PRIVATE cxx_std_20)
    target_compile_options(example
                        PRIVATE
                        $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
    target_link_libraries(example PRIVATE crepuscule)
endif()

# Tests
if (TESTS)
    include(CTest)
    enable_testing(MAKE_DIRECTORY test)
    add_subdirectory(test)
endif()