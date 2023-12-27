# Add the path to your custom libraries
include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")


foreach(src_file ${SRC_FILES})
    get_filename_component(target ${src_file} NAME_WLE)
    add_executable(${target} ${src_file})
    list(APPEND TARGETS ${target})
    message("target added: ${target}")
endforeach()


foreach(target ${TARGETS})
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
        CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PRIVATE 
            -g
            -Wall
            -Wextra
            -Werror
            -pedantic
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE
            /EHsc
            /W4
            /WX
        )
    endif()
    target_compile_features(${target} PUBLIC cxx_std_17)
endforeach()
