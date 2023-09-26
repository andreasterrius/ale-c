#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>
#include <raymath.h>
#include "gizmo.h"

typedef struct Object {
    Transform transform;
    Model model;
} Object;

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

    AleGizmo gizmo;
    alGizmoInit(&gizmo);

    while (!WindowShouldClose()) {
//        UpdateCamera(&camera, CAMERA);
        {
            BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawFPS(10, 10);

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                alGizmoTryHold(&gizmo, &object.transform, camera);
            } else {
                alGizmoRelease(&gizmo);
            }

            DrawText("gizmo demo!", 100, 100, 20, YELLOW);
            BeginMode3D(camera);

            alGizmoRender(gizmo);

            Vector3 outAxis;
            float outAngle;

            QuaternionToAxisAngle(object.transform.rotation, &outAxis, &outAngle);
            DrawModelEx(object.model, object.transform.translation, outAxis, outAngle, object.transform.scale, WHITE);
            //DrawModel(object.model, object.transform.translation, 1.0f, WHITE);
            DrawGrid(10, 1.0f);

            EndMode3D();
            EndDrawing();
        }
    }

    alGizmoDeinit(&gizmo);
    UnloadModel(object.model);
    CloseWindow();
    return 0;
}
