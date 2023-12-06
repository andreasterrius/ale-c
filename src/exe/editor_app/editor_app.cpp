#include<raylib.h>
#include<raymath.h>
#include"../../object.h"
#include"../../scene_editor.h"
//#include "src/ui/model_ui.h"
#include"../../unicode_font.h"
#include"../../rldata.h"
#include<vector>

int main(int argc, char **argv) {
    i32 errCode = 0;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Vector2 windowSize = (Vector2) {.x = 800, .y=800};
    InitWindow(windowSize.x, windowSize.y, "Hello Editor");

    SetTargetFPS(60);

    Camera3D camera;
    camera.position = (Vector3) {2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float sceneAspectRatio = windowSize.x / windowSize.y;
    Rectangle sceneRect = (Rectangle) {.height=0.8f * sceneAspectRatio, .width=0.8f, .x=0.0f, .y=0.0f};
    Rectangle modelUiRect = (Rectangle) {.height=1.0f * sceneAspectRatio, .width=0.2f, .x=sceneRect.width, .y=0.0f};

    // Load available models
    AlSceneEditor sceneEditor(camera, sceneRect);

    std::vector<AlUnicodeFontRange> ranges{
            (AlUnicodeFontRange) {.start = 0, .end=255}
    };
    AlUnicodeFont unicodeFont("resources/font/NotoSansCJKjp-VF.ttf", 24, ranges);

    sceneEditor.objects.emplace_back(
            Transform{Vector3Zero(), QuaternionIdentity(), Vector3One()},
            RlModel{LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 1.0f))},
            true
    );
    sceneEditor.objects.emplace_back(
            Transform{Vector3Zero(), QuaternionIdentity(), Vector3One()},
            RlModel{LoadModelFromMesh(GenMeshSphere(2.0f, 16, 16))},
            true
    );

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        sceneEditor.handleInput();
        sceneEditor.tick(deltaTime);
//        alModelUiTick(&modelUi);

        sceneEditor.render();
//        alModelUiRender(&modelUi);

        BeginDrawing();
        {

            ClearBackground(SKYBLUE);
            sceneEditor.renderRtt();
//            alModelUiRenderRtt(&modelUi);
        }
        EndDrawing();

        DrawGrid(10, 1.0f);
    }

    CloseWindow();

    return errCode;
}
