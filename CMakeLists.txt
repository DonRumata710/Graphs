
cmake_minimum_required(VERSION 3.0)

project(Graphs)

macro(set_option var type default docstring) 
    if(NOT DEFINED ${var}) 
        set(${var} ${default}) 
    endif() 
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE) 
endmacro() 

set_option(QWT_ROOT "${CMAKE_SOURCE_DIR}/3rd_party/qwt" "Set QWT folder")

if (WIN32) 
    set(SFML_STATIC_LIBRARIES TRUE) 
else() 
    set(SFML_STATIC_LIBRARIES FALSE) 
endif() 

add_subdirectory(qwt)
add_subdirectory(graphs)
