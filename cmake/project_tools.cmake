include(helpers)

# Enable formatting of source files by using clang-format.
macro(project_enable_clang_format)
    if (NOT DEFINED PROJECT_FILES)
        project_get_files_of_subdirectories(STORE_AS PROJECT_FILES)
    endif()
    
    find_program(PROJECT_CLANG_FORMAT clang-format)

    if (PROJECT_CLANG_FORMAT)
        add_custom_target(format
            COMMAND ${PROJECT_CLANG_FORMAT} -i ${PROJECT_FILES}
        )
        message(STATUS "clang-format was found. Formatting can be done with cmake --build build --target=format")
    else()
        message(STATUS "clang-format was not found. No custom target will be defined for it.")
    endif()    
endmacro()

# Enables static analysis of source files by using clang-tidy.
macro(project_enable_clang_tidy)
    if (NOT DEFINED PROJECT_FILES)
        project_get_files_of_subdirectories(STORE_AS PROJECT_FILES)
    endif()

    find_program(PROJECT_CLANG_TIDY clang-tidy)

    if (PROJECT_CLANG_TIDY)
        add_custom_target(lint
            COMMAND ${PROJECT_CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${PROJECT_FILES}
        )
        message(STATUS "clang-tidy was found. Linting can be done with cmake --build build --target=lint")
    else()
        message(STATUS "clang-tidy was not found. No custom target will be defined for it.")
    endif()
endmacro()