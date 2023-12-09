#include<raylib.h>
#include<raymath.h>
#include"../../object.h"
#include"../../scene_editor.h"
#include "../../ui/model_ui.h"
#include"../../unicode_font.h"
#include"../../rldata.h"
#include"../../scene_file_loader.h"
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

    std::shared_ptr<AlUnicodeFont> unicodeFont = std::make_shared<AlUnicodeFont>(
            "resources/font/NotoSansCJKjp-VF.ttf",
            24,
            std::vector{AlUnicodeFontRange{.start = 0, .end=255}}
    );

    // Let's do save and load here
    AlSceneFileLoader sceneFileLoader("");

    // Load available models
    AlSceneEditor sceneEditor(camera, sceneRect);
    sceneEditor.saveScenePath = "resources/scenes/editor01.json";

    AlModelUi modelUi(unicodeFont, modelUiRect, "resources/models");
    modelUi.loadedModelEntries.emplace_back(AlModelUi_Entry{
            "cube",
            std::make_shared<RlModel>(RlModel{LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 1.0f))})
    });
    modelUi.loadedModelEntries.emplace_back(AlModelUi_Entry{
            "sphere",
            std::make_shared<RlModel>(RlModel{ LoadModelFromMesh(GenMeshSphere(2.0f, 16, 16))})
    });

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        sceneEditor.handleInput();
        sceneEditor.tick(deltaTime);

        modelUi.tick(deltaTime);
        std::vector<AlObject> objects = modelUi.getSpawnCommands();
        if(!objects.empty())
            sceneEditor.objects.insert(sceneEditor.objects.end(), objects.begin(), objects.end());

        sceneEditor.render();
        modelUi.render();

        BeginDrawing();
        {
            ClearBackground(SKYBLUE);
            sceneEditor.renderRtt();
            modelUi.renderRtt();
        }
        EndDrawing();

        DrawGrid(10, 1.0f);
    }

    CloseWindow();

    return errCode;
}
