#include <raylib.h>
#include <raymath.h>
#include "object.h"
#include "scene_editor.h"

int main(int argc, char **argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Hello Gizmo");
    SetTargetFPS(60);

    AlSceneEditor sceneEditor;
    alSceneEditorInit(&sceneEditor);

    Camera3D camera;
    camera.position = (Vector3) {2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    AlObject object;
    object.transform = (Transform) {
            .translation = Vector3Zero(),
            .rotation = QuaternionIdentity(),
            .scale = Vector3One(),
    };
    object.model = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 1.0f));

//    AlObject anotherObject;
//    anotherObject.transform = (Transform) {
//            .translation = (Vector3) {.x=3, .y=3, .z=3},
//            .scale = Vector3One(),
//            .rotation = QuaternionIdentity()
//    };
//    anotherObject.model = LoadModelFromMesh(GenMeshSphere(2.0f, 16, 16));

    // let's add this manually for now
    g_array_append_val(sceneEditor.objects, object);
//    g_array_append_val(sceneEditor.objects, anotherObject);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(10, 10);

        DrawText("scene editor demo!", 100, 100, 20, YELLOW);
        BeginMode3D(camera);

        alSceneEditorHandleInput(&sceneEditor, &camera);
        alObjectTryRecalculate(&object);

        alSceneEditorRender(&sceneEditor);

        DrawGrid(10, 1.0f);

        EndMode3D();
        EndDrawing();
    }

    alSceneEditorDeinit(&sceneEditor);
    CloseWindow();
    return 0;
}
