add_executable(rtt_demo rtt_demo.cpp
        ../../rtt.cpp
        ../../rtt.h
        ../../arc_camera.cpp
        ../../arc_camera.h
)

target_include_directories(rtt_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(rtt_demo PRIVATE raylib)