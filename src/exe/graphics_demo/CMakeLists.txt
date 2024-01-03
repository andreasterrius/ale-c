add_executable(graphics_demo graphics_demo.cpp
        ../../arc_camera.cpp
        ../../arc_camera.h
        ../../rldata.h
)

target_include_directories(graphics_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(graphics_demo PRIVATE raylib)