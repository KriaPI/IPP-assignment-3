if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /W4 /w14640 /permissive-
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if (CMAKE_BUILD_TYPE MATCHES "Debug") 
        add_compile_options(
            -Wall -Wextra -Wpedantic
        )
    else()
        add_compile_options(
            -Wall -Wextra -Wpedantic -O3
        )
    endif()
endif()
