option(GENERATE_CODE_COVERAGE "Generate code coverage using gcov and process it using gcovr (GCC ONLY)" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND ${GENERATE_CODE_COVERAGE} MATCHES ON)
    # This specifies needed compiler and linker options for gcov when compiling with gcc.
    add_link_options(--coverage)
    add_compile_options(--coverage -fprofile-abs-path)

    # This replaces the gcovr config variables beginning and ending with @, which
    # is neccesary to provide things such as the path to gcov and cmake source directory. 
    find_program(GCOV_EXECUTABLE gcov)
    find_program(GCOVR_EXECUTABLE gcovr)
    configure_file(gcovr.cfg.in gcovr.cfg @ONLY)

    if (NOT DEFINED GCOV_EXECUTABLE) 
        message(STATUS "Code coverage could not be generated, gcov was not found.")
    endif()

    if (NOT DEFINED GCOVR_EXECUTABLE) 
        message(STATUS "Code coverage could not be generated, gcovr was not found.")
    endif()

    if (DEFINED GCOV_EXECUTABLE AND DEFINED GCOVR_EXECUTABLE)
        add_custom_target(coverage
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage"
            COMMAND cmake --build ${CMAKE_BINARY_DIR}
            COMMAND ctest --test-dir ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/coverage/html"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/coverage/sonarqube"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/coverage/cobertura"
            COMMAND ${GCOVR_EXECUTABLE} --config gcovr.cfg .
        )
    endif()
endif()
