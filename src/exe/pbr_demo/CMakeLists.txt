add_executable(pbr_demo pbr_demo.cpp
        ../../arc_camera.cpp
        ../../arc_camera.h
)

target_include_directories(pbr_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(pbr_demo PRIVATE raylib)