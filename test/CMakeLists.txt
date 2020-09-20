add_executable(first_test ./first_test.cpp)
target_compile_features(first_test PRIVATE cxx_std_20)
target_compile_options(first_test
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -g>)
target_link_libraries(first_test PRIVATE crepuscule)                       
target_include_directories(first_test PRIVATE ${CMAKE_CURRENT_LIST_DIR})
add_test(first first_test)
                      