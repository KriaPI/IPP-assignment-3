# Retrieve targets from all subdirectories
function(project_get_targets_of_subdirectories)
    set(prefix ARG)
    set(singleValues STORE_AS)
    cmake_parse_arguments(
        PARSE_ARGV 0
        ${prefix}
        "" "${singleValues}" ""  
    )
    
    get_directory_property(projectSubdirectories SUBDIRECTORIES)

    foreach(subdirectory ${projectSubdirectories})
        get_property(subdirectoryTargets
            DIRECTORY ${subdirectory}
            PROPERTY BUILDSYSTEM_TARGETS
        )
        list(APPEND projectTargets ${subdirectoryTargets})
    endforeach()
    
    list(GET singleValues 0 result)
    set(${${prefix}_${result}} ${projectTargets} PARENT_SCOPE)
endfunction()

# Retrieve sources of all targets within subdirectories relative to the top-level directory
# of the cmake project
function(project_get_files_of_subdirectories)
    set(prefix ARG)
    set(singleValues STORE_AS)
    cmake_parse_arguments(
        PARSE_ARGV 0
        ${prefix}
        "" "${singleValues}" ""  
    )

    project_get_targets_of_subdirectories(STORE_AS projectTargets)

    foreach(target ${projectTargets})
        get_target_property(targetFiles ${target} SOURCES)
        get_target_property(sourceDir ${target} SOURCE_DIR)

        foreach(file ${targetFiles})
            cmake_path(APPEND fullPath ${sourceDir} ${file})
            list(APPEND files ${fullPath})
        endforeach()
    endforeach()
    list(REMOVE_DUPLICATES files)

    list(GET singleValues 0 result)
    set(${${prefix}_${result}} ${files} PARENT_SCOPE)
endfunction()