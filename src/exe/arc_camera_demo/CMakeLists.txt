## arc_camera
add_executable(arc_camera_demo arc_camera_demo.cpp
        ../../arc_camera.h
        ../../arc_camera.cpp)
target_include_directories(arc_camera_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(arc_camera_demo PRIVATE raylib)
