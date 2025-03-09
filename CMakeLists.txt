cmake_minimum_required(VERSION 3.14)

project(wifisetup VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Quick)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC, adding compile warning options")
    set(COMPILER_NAME "gnu")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Using MSVC")
    set(COMPILER_NAME "msvc")
    add_compile_options(/Zc:__cplusplus /permissive- /W4)
else()
    message(FATAL_ERROR "Using unknown compiler")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" CONFIGURATION)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/${COMPILER_NAME}_${CONFIGURATION})

file(MAKE_DIRECTORY ${OUTPUT_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

add_subdirectory(sources)
