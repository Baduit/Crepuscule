set(SRCS
    ./crepuscule/crepuscule.cpp
    ./crepuscule/helpers.cpp
    ./crepuscule/ProcessingState.cpp
    )


add_library(crepuscule ${SRCS})

target_compile_features(crepuscule PRIVATE cxx_std_20)

target_compile_options(crepuscule
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)

target_include_directories(crepuscule PUBLIC ${CMAKE_CURRENT_LIST_DIR})