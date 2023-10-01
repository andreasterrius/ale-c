//
// Created by alether on 10/2/23.
//
#include<raylib.h>
#include<raymath.h>
#include"arc_camera.h"

// Adjusted for a corrected viewport drawing.
Vector2 GetMousePositionCorrected(Vector2 oldSize, Vector2 offset, Vector2 newSize) {
    Vector2 mousePos = GetMousePosition();
    Vector2 resultPos = (Vector2){
        .x = oldSize.x / newSize.x * (offset.x+mousePos.x),
        .y = oldSize.y / newSize.y * (offset.y+mousePos.y)
    };
    return resultPos;
}

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

    AlArcCameraInput alArcCameraInput;
    alArcCameraInputInit(&alArcCameraInput);

    Model model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    RenderTexture2D target = LoadRenderTexture(600, 400);

    //Vector2 offset = Vector2Zero();
    Vector2 offset = (Vector2) {.x = -200, .y = -200};
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
            Vector2 mousePos = GetMousePositionCorrected(
                    (Vector2) {.x=GetScreenWidth(), .y=GetScreenHeight()},
                    offset,
                    (Vector2) {.x=target.texture.width, .y=target.texture.height});
            ray = GetMouseRay(mousePos, camera);
        }

        {
            BeginTextureMode(target);
            ClearBackground(SKYBLUE);
            DrawFPS(10, 10);
            DrawText("viewport demo!", 100, 100, 20, YELLOW);

            BeginMode3D(camera);
            DrawRay(ray, RED);

            DrawModel(model, Vector3Zero(), 1.0f, WHITE);

            DrawGrid(10, 1.0f);

            EndMode3D();
            EndTextureMode();
        }

        {
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexturePro(target.texture,
                           (Rectangle) {0.0f, 0.0f, (float) target.texture.width, -target.texture.height},
                           (Rectangle) {0.0f, 0.0f, (float) target.texture.width, target.texture.height},
                           offset, 0.0f, WHITE);

            EndDrawing();
        }
    }

    UnloadRenderTexture(target);
    UnloadModel(model);
    CloseWindow();
    return 0;
}