cmake_minimum_required (VERSION 2.8.11)

project(GeneDrop)
set(GeneDrop_VERSION_MAJOR 1)
set(GeneDrop_VERSION_MINOR 0)

# TODO: Work out the minimum boost version needed
find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Fatal error: Boost required.\n")
endif(NOT Boost_FOUND)

cmake_policy(SET CMP0008 OLD)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(GeneDropBackEnd)

add_subdirectory(GeneDropBackEnd)
add_subdirectory(GeneDropCommandLineFrontEnd)
add_subdirectory(GeneDropTests)
