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

    RlShader shader;
    shader.d = LoadShader(NULL, "resources/shaders/pbr.fs");
    shader.d.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader.d, "cameraPosition");

    int lightPosLoc = GetShaderLocation(shader.d, "lightPos");
    int lightColorLoc = GetShaderLocation(shader.d, "lightColor");

    Vector3 lightPos = Vector3Zero();

    Color lightColor = WHITE;

    AlArcCameraInput alArcCameraInput;
    Model model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    model.materials->shader = shader.d;

    Ray ray = GetMouseRay(GetMousePosition(), camera);
    while (!WindowShouldClose()) {

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            HideCursor();
            alArcCameraInput.tryArcBall(&camera);
        } else {
            ShowCursor();
            alArcCameraInput.releaseArcBall();
        }

        {
            BeginDrawing();

            DrawFPS(10, 10);
            DrawText("viewport demo!", 100, 100, 20, YELLOW);

            ClearBackground(ColorAlpha(SKYBLUE, 1.0f));
            BeginMode3D(camera);

            DrawRay(ray, RED);
            DrawModel(model, Vector3Zero(), 1.0f, WHITE);

            DrawGrid(10, 1.0f);
            EndMode3D();
            EndDrawing();
        }
    }

    UnloadModel(model);
    CloseWindow();

    return 0;
}