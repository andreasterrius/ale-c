add_executable(gizmo_demo gizmo_demo.cpp
        ../../gizmo.h
        ../../gizmo.cpp
        ../../object.cpp
        ../../object.h)
target_include_directories(gizmo_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(gizmo_demo PRIVATE raylib)