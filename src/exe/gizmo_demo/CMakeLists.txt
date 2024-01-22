add_executable(gizmo_demo gizmo_demo.cpp
        ../../gizmo.h
        ../../gizmo.cpp
        ../../pbr_shader.cpp
        ../../pbr_shader.h
        ../../rlmath.cpp
        ../../rlmath.h
        ../../object.cpp
        ../../object.h)
target_include_directories(gizmo_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(gizmo_demo PRIVATE raylib)