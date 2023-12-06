add_executable(text_renderer unicode_font_demo.cpp
        ../../unicode_font.h
        ../../unicode_font.cpp
        ../../rldata.h
        ../../types.h)

target_include_directories(text_renderer PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(text_renderer PRIVATE raylib)