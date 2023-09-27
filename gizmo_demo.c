#include <raylib.h>
#include <raymath.h>
#include "gizmo.h"
#include "object.h"

int main(int argc, char **argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Hello Gizmo");
    SetTargetFPS(60);

    Camera3D camera;
    camera.position = (Vector3) {2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Object object;
    object.transform = (Transform) {
            .translation = Vector3Zero(),
            .rotation = QuaternionIdentity(),
            .scale = Vector3One(),
    };
    object.model = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 1.0f));
    object.transform = (Transform) {.translation = Vector3Zero(), .scale = Vector3One(), .rotation = QuaternionIdentity()};

    AleGizmo gizmo;
    alGizmoInit(&gizmo);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(10, 10);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (alGizmoTryHold(&gizmo, &object.transform, camera)) {
                object.hasTransformChanged = true;
            }
        } else {
            alGizmoRelease(&gizmo);
        }

        DrawText("gizmo demo!", 100, 100, 20, YELLOW);
        BeginMode3D(camera);

        alObjectTryRecalculate(&object);
        alGizmoRender(gizmo);

        DrawModel(object.model, Vector3Zero(), 1.0f, WHITE);
        DrawGrid(10, 1.0f);

        EndMode3D();
        EndDrawing();
    }

    alGizmoDeinit(&gizmo);
    UnloadModel(object.model);
    CloseWindow();
    return 0;
}
