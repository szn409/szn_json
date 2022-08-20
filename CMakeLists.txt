cmake_minimum_required(VERSION 3.0.0)
project(szn_json VERSION 0.1.0)

add_compile_options(/wd"4996" /wd"4819")

add_compile_definitions(_WIN32_WINNT=0x0601)

file(GLOB_RECURSE all_cpp ./szn_json/*.cpp)
file(GLOB_RECURSE all_h ./szn_json/*.h)
file(GLOB_RECURSE all_hpp ./szn_json/*.hpp)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)
 
function(my_add_executable)
    foreach(_source IN ITEMS ${ARGN})
        assign_source_group(${_source})
    endforeach()
endfunction(my_add_executable)

my_add_executable(szn_json ${all_cpp})
my_add_executable(szn_json ${all_h})
my_add_executable(szn_json ${all_hpp})

add_executable(szn_json main.cpp ${all_cpp} ${all_h} ${all_hpp})