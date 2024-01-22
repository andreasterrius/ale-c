cmake_minimum_required(VERSION 3.26)
project(hello_c C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

## dependencies
add_subdirectory(extern/raylib)
add_subdirectory(extern/json)

enable_testing()
add_subdirectory(src/data_old)

add_subdirectory(src/exe/editor_app)
add_subdirectory(src/exe/arc_camera_demo)
add_subdirectory(src/exe/rtt_demo)
add_subdirectory(src/exe/gizmo_demo)
add_subdirectory(src/exe/unicode_font_demo)
add_subdirectory(src/exe/playground)
add_subdirectory(src/exe/pbr_demo)
add_subdirectory(src/exe/model_ui_demo)