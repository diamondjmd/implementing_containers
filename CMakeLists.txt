cmake_minimum_required(VERSION 3.5)

project(project_containers)

add_executable(testing_list  main_list.cpp)

add_executable(testing_vector  main_vector.cpp)

add_executable(testing_forward_list main_forward_list.cpp)

set_target_properties(testing_list testing_vector testing_forward_list PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(testing_list  PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(testing_list PRIVATE
        /EHsc
        /W4
        /WX
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(testing_vector PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(testing_vector PRIVATE
        /EHsc
        /W4
        /WX
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(testing_forward_list PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(testing_forward_list PRIVATE
        /EHsc
        /W4
        /WX
    )
endif()
