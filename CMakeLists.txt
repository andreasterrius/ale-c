cmake_minimum_required(VERSION 3.26)
project(hello_c C)

set(CMAKE_C_STANDARD 23)

## glib
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

## raylib
add_subdirectory(extern/raylib)

## hello_c
add_executable(gizmo_demo gizmo_demo.c
        gizmo.h
        gizmo.c)
target_include_directories(gizmo_demo PRIVATE ${GLIB_INCLUDE_DIRS} ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(gizmo_demo PRIVATE ${GLIB_LIBRARIES} raylib)
