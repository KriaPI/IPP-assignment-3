# Options for enabling santizers such as Address Sanitizer (ASAN), Thread Sanitizer (TSAN)
# and Undefined Behavior Sanitizer (UBSAN). TSAN and UBSAN cannot be enabled at the same time.
# Note that MSVC only supports ASAN and that TSAN may not work with GCC (version 13.2).


option(ENABLE_ASAN "Enable address sanitizer" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND ENABLE_ASAN MATCHES ON)
    string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    add_link_options(
        /INCREMENTAL:NO   
    )
    add_compile_options(
        /fsanitize=address 
    )
elseif ((CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")) 
    option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer (does not work with Address Sanitizer (ASAN))" OFF)
    option(ENABLE_TSAN "Enable thread sanitizer" OFF)

    if (ENABLE_ASAN MATCHES ON AND ENABLE_TSAN MATCHES ON)
        message(FATAL_ERROR "Thread Sanitizer (TSAN) and Address Sanitizer (ASAN) cannot be enabled at the same time. Disable one of them to resolve this issue.")
    endif()

    add_link_options(
        $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>
        $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>
        $<$<BOOL:${ENABLE_TSAN}>:-fsanitize=thread>
    )    
    add_compile_options(
        $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>
        $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>
        $<$<BOOL:${ENABLE_TSAN}>:-fsanitize=thread>
    ) 
endif()