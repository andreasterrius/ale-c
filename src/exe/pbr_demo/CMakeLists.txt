add_executable(pbr_demo pbr_demo.cpp
        ../../arc_camera.cpp
        ../../arc_camera.h
        ../../pbr_shader.cpp
        ../../pbr_shader.h
        ../../rlmath.h
        ../../rlmath.cpp
        ../../light.h
        ../../light.cpp
        ../../object.h
        ../../object.cpp
)

target_include_directories(pbr_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(pbr_demo PRIVATE raylib)
