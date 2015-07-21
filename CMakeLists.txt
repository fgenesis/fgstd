cmake_minimum_required(VERSION 2.6)
project(fgstd)


# non-msvc needs build type - if no build type was provided, set a default one
if(NOT MSVC)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
    endif()
endif()

include_directories(.)

add_subdirectory(fgstd)
add_subdirectory(test)

