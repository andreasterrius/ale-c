//
// Created by alether on 10/2/23.
//
#include<raylib.h>
#include<raymath.h>
#include<stdio.h>
#include"rtt.h"
#include"arc_camera.h"
#include"defer.h"

int main(int argc, char **argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Vector2 windowSize = (Vector2) {.x = 800, .y = 800};
    InitWindow(windowSize.x, windowSize.y, "Hello render to texture");
    defer{ CloseWindow(); };

    SetTargetFPS(60);

    Camera3D camera;
    camera.position = (Vector3) {2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    AlArcCameraInput alArcCameraInput;
    alArcCameraInputInit(&alArcCameraInput);

    Model model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    defer{ UnloadModel(model); };

    AlRtt viewport;
    alRttInit(&viewport, (Rectangle) {.x = 0, .y = 0, .width=1.0, .height=1.0});
    defer{ alRttDeinit(&viewport); };

    Ray ray = GetMouseRay(GetMousePosition(), camera);
    while (!WindowShouldClose()) {

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            HideCursor();
            alArcCameraInputTryArcBall(&alArcCameraInput, &camera);
        } else {
            ShowCursor();
            alArcCameraInputReleaseArcBall(&alArcCameraInput);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            ray = alRttGetMouseRay(viewport, camera);
        }

        {
            alRttBeginRenderToTexture(viewport);
            defer{ alRttEndRenderToTexture(viewport); };

            DrawFPS(10, 10);
            DrawText("viewport demo!", 100, 100, 20, YELLOW);

            ClearBackground(ColorAlpha(SKYBLUE, 0.0f));
            BeginMode3D(camera);
            defer{ EndMode3D(); };
            DrawRay(ray, RED);

            DrawModel(model, Vector3Zero(), 1.0f, WHITE);

            DrawGrid(10, 1.0f);
        }

        {
            BeginDrawing();
            defer{ EndDrawing(); };

            alRttRenderTexture(viewport);

            ClearBackground(SKYBLUE);
        }
    }

    return 0;
}