//
// Created by alether on 10/2/23.
//
#include<raylib.h>
#include<raymath.h>
#include"../../rtt.h"
#include"../../arc_camera.h"

int main(int argc, char **argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Vector2 windowSize = (Vector2) {.x = 800, .y = 800};
    InitWindow(windowSize.x, windowSize.y, "Hello render to texture");

    SetTargetFPS(60);

    Camera3D camera;
    camera.position = (Vector3) {2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    AlArcCameraInput alArcCameraInput;

    Model model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

    AlRtt viewport((Rectangle) {.x = 0.3, .y = 0.4, .width=0.5, .height=0.5});

    Ray ray = GetMouseRay(GetMousePosition(), camera);
    while (!WindowShouldClose()) {

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            HideCursor();
            alArcCameraInput.tryArcBall(&camera);
        } else {
            ShowCursor();
            alArcCameraInput.releaseArcBall();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            ray = viewport.getMouseRay(camera);
        }

        {
            viewport.beginRenderToTexture();

            DrawFPS(10, 10);
            DrawText("viewport demo!", 100, 100, 20, YELLOW);

            ClearBackground(ColorAlpha(SKYBLUE, 1.0f));
            BeginMode3D(camera);

            DrawRay(ray, RED);
            DrawModel(model, Vector3Zero(), 1.0f, WHITE);

            DrawGrid(10, 1.0f);
            EndMode3D();
            viewport.endRenderToTexture();
        }

        {
            BeginDrawing();
            viewport.renderTexture();
            ClearBackground(BLACK);
            EndDrawing();
        }
    }

    UnloadModel(model);
    CloseWindow();

    return 0;
}