cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(word-stats)

option(BUILD_TESTING "Build tests" OFF)

include(cmake/configure.cmake)

add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
