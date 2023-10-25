cmake_minimum_required(VERSION 3.26)
project(hello_c C)

set(CMAKE_C_STANDARD 23)

## dependencies
add_subdirectory(extern/raylib)
add_subdirectory(extern/json-c)
add_subdirectory(extern/xxHash/cmake_unofficial EXCLUDE_FROM_ALL)
add_subdirectory(extern/blocksruntime)
add_subdirectory(extern/klib)
add_subdirectory(extern/googletest)

## googletest


## gizmo_demo
add_executable(gizmo_demo gizmo_demo.c
        gizmo.h
        gizmo.c
        object.c
        object.h)
target_include_directories(gizmo_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(gizmo_demo PRIVATE raylib)

## arc_camera
add_executable(arc_camera_demo
        arc_camera.h
        arc_camera.c
        arc_camera_demo.c)
target_include_directories(arc_camera_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(arc_camera_demo PRIVATE raylib)

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
        rtt.h
        ui.h
        ui.c
)
target_compile_options(scene_editor PRIVATE -fblocks)
target_include_directories(scene_editor PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(scene_editor PRIVATE raylib BlocksRuntime klib)

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
target_include_directories(rtt_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(rtt_demo PRIVATE raylib BlocksRuntime)

## ui_demo
add_executable(ui_demo
        ui_demo.c
        ui.h
        ui.c
)

target_compile_options(ui_demo PRIVATE -fblocks)
target_include_directories(ui_demo PRIVATE ${RAYLIB_INCLUDE_DIRS} ${JSON_C_INCLUDE_DIRS})
target_link_libraries(ui_demo PRIVATE raylib BlocksRuntime json-c)

## text_renderer_demo
add_executable(text_renderer
        unicode_font_demo.c
        unicode_font.h
        unicode_font.c)

target_compile_options(text_renderer PRIVATE -fblocks)
target_include_directories(text_renderer PRIVATE ${RAYLIB_INCLUDE_DIRS} ${JSON_C_INCLUDE_DIRS} xxHash::xxhash)
target_link_libraries(text_renderer PRIVATE raylib BlocksRuntime json-c xxHash::xxhash klib)


## interface demo
add_executable(interface_demo interface_demo.c)
