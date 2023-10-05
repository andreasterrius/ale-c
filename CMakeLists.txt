cmake_minimum_required(VERSION 3.26)
project(hello_c C)

set(CMAKE_C_STANDARD 23)

## glib
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

## raylib
add_subdirectory(extern/raylib)

## gizmo_demo
add_executable(gizmo_demo gizmo_demo.c
        gizmo.h
        gizmo.c
        object.c
        object.h)
target_include_directories(gizmo_demo PRIVATE ${GLIB_INCLUDE_DIRS} ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(gizmo_demo PRIVATE ${GLIB_LIBRARIES} raylib)

## arc_camera
add_executable(arc_camera_demo
        arc_camera.h
        arc_camera.c
        arc_camera_demo.c)
target_include_directories(arc_camera_demo PRIVATE ${GLIB_INCLUDE_DIRS} ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(arc_camera_demo PRIVATE ${GLIB_LIBRARIES} raylib)

## scene_editor
add_executable(scene_editor
        scene_editor_demo.c
        arc_camera.h
        arc_camera.c
        gizmo.h
        gizmo.c
        object.c
        object.h
        scene_editor.c
        scene_editor.h
        rtt.c
        rtt.h)
target_compile_options(scene_editor PRIVATE -fblocks)
target_include_directories(scene_editor PRIVATE ${GLIB_INCLUDE_DIRS} ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(scene_editor PRIVATE ${GLIB_LIBRARIES} raylib BlocksRuntime)

## viewport_demo
add_executable(rtt_demo
        rtt_demo.c
        rtt.c
        rtt.h
        arc_camera.c
        arc_camera.h
        defer.h
)

target_compile_options(rtt_demo PRIVATE -fblocks)
target_include_directories(rtt_demo PRIVATE ${GLIB_INCLUDE_DIRS} ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(rtt_demo PRIVATE ${GLIB_LIBRARIES} raylib BlocksRuntime)