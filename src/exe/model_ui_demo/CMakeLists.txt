add_executable(model_ui_demo model_ui_demo.cpp
        ../../rtt.cpp
        ../../rtt.h
        ../../ui/button.h
        ../../ui/button.cpp
        ../../ui/model_ui.cpp
        ../../ui/model_ui.h
        ../../ui/model_preview.cpp
        ../../ui/model_preview.h
        ../../rldata.h
        ../../rlmath.h
        ../../rlmath.cpp
        ../../rljson.h
        ../../unicode_font.h
        ../../unicode_font.cpp
        ../../object.h
        ../../object.cpp
)

target_include_directories(model_ui_demo PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(model_ui_demo PRIVATE raylib)