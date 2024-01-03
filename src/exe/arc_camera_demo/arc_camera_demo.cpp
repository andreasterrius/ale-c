#include<raylib.h>
#include<raymath.h>
#include"../../arc_camera.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Hello Gizmo");
    SetTargetFPS(60);

    Camera3D camera;
    camera.position = Vector3 {2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = Vector3 {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model model = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));

    AlArcCameraInput alArcCameraInput;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(10, 10);

        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            HideCursor();
            alArcCameraInput.tryArcBall(&camera);
        } else {
            ShowCursor();
            alArcCameraInput.releaseArcBall();
        }
        alArcCameraInput.zoomOut(&camera);

        DrawText("arc camera demo!", 100, 100, 20, YELLOW);
        BeginMode3D(camera);

        DrawModel(model, Vector3Zero(), 1.0f, WHITE);
        DrawGrid(10, 1.0f);

        EndMode3D();
        EndDrawing();
    }

    UnloadModel(model);
    CloseWindow();
    return 0;
}