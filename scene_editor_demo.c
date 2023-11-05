#include <raylib.h>
#include <raymath.h>
#include "object.h"
#include "scene_editor.h"
#include "defer.h"

int main(int argc, char **argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Vector2 windowSize = (Vector2) {.x = 800, .y=800};
    InitWindow(windowSize.x, windowSize.y, "Hello Editor");
    defer{ CloseWindow(); };

    SetTargetFPS(60);

    Camera3D camera;
    camera.position = (Vector3) {2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    AlSceneEditor sceneEditor;
    alSceneEditorInit(&sceneEditor, camera);
    defer{ alSceneEditorDeinit(&sceneEditor); };

    AlObject object;
    object.transform = (Transform) {
            .translation = Vector3Zero(),
            .rotation = QuaternionIdentity(),
            .scale = Vector3One(),
    };
    object.model = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 1.0f));
    object.hasTransformChanged = true;

    AlObject anotherObject;
    anotherObject.transform = (Transform) {
            .translation = (Vector3) {.x=3, .y=3, .z=3},
            .scale = Vector3One(),
            .rotation = QuaternionIdentity()
    };
    anotherObject.model = LoadModelFromMesh(GenMeshSphere(2.0f, 16, 16));
    anotherObject.hasTransformChanged = true;

    // let's add this manually for now
    alArrayPush(&sceneEditor.objects, &object);
    alArrayPush(&sceneEditor.objects, &anotherObject);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        alSceneEditorHandleInput(&sceneEditor);
        alSceneEditorTick(&sceneEditor, deltaTime);

        alSceneEditorRender(&sceneEditor);

        DrawGrid(10, 1.0f);
    }

    return 0;
}
