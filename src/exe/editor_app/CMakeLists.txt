## scene_editor
add_executable(editor_app editor_app.cpp
        ../../arc_camera.h
        ../../arc_camera.cpp
        ../../gizmo.h
        ../../gizmo.cpp
        ../../object.cpp
        ../../object.h
        ../../scene_editor.cpp
        ../../scene_editor.h
        ../../rtt.cpp
        ../../rtt.h
        ../../ui/model_ui.h
        ../../ui/model_ui.cpp
        ../../ui/button.h
        ../../ui/button.cpp
        ../../unicode_font.cpp
        ../../unicode_font.h
        ../../rldata.h
#        src/ui/button.c
#        src/ui/button.h
)
target_include_directories(editor_app PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(editor_app PRIVATE raylib)