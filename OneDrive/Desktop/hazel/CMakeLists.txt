cmake_minimum_required(VERSION 3.20)

project(hazel LANGUAGES C)

if(NOT CMAKE_GENERATOR MATCHES "Ninja")
    message(FATAL_ERROR "Please configure this project with Ninja: cmake -S . -B build -G Ninja")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE HAZEL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/*.c"
)

file(GLOB_RECURSE HAZEL_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/*.h"
)

foreach(source_file IN LISTS HAZEL_SOURCES HAZEL_HEADERS)
    if(source_file MATCHES "^${CMAKE_BINARY_DIR}/")
        list(REMOVE_ITEM HAZEL_SOURCES "${source_file}")
        list(REMOVE_ITEM HAZEL_HEADERS "${source_file}")
    endif()
endforeach()

set(HAZEL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}")

foreach(header_file IN LISTS HAZEL_HEADERS)
    get_filename_component(header_dir "${header_file}" DIRECTORY)
    list(APPEND HAZEL_INCLUDE_DIRS "${header_dir}")
endforeach()

list(REMOVE_DUPLICATES HAZEL_INCLUDE_DIRS)

add_executable(${PROJECT_NAME}
    ${HAZEL_SOURCES}
    ${HAZEL_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${HAZEL_INCLUDE_DIRS}
)

find_library(RAYLIB_LIBRARY
    NAMES raylib libraylib
    PATHS
        "C:/raylib/raylib/src"
        "C:/raylib/w64devkit/lib"
)

if(NOT RAYLIB_LIBRARY)
    message(FATAL_ERROR "raylib library was not found. Install raylib or update RAYLIB_LIBRARY paths in CMakeLists.txt.")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    "${RAYLIB_LIBRARY}"
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        gdi32
        winmm
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
