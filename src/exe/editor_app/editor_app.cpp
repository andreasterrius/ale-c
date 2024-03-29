#include<raylib.h>
#include<raymath.h>
#include"../../object.h"
#include"../../scene_editor.h"
#include"../../ui/model_ui.h"
#include"../../light.h"
#include"../../pbr_shader.h"
#include"../../scene_file_loader.h"
#include"../../default_stuff.h"
#include<vector>

int main(int argc, char **argv) {
    i32 errCode = 0;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Vector2 windowSize = Vector2{.x = 800, .y=800};
    InitWindow(windowSize.x, windowSize.y, "Hello Editor");

    SetTargetFPS(60);

    Camera3D camera;
    camera.position = Vector3{2.0f, 4.0f, -10.0f};
    camera.target = Vector3Zero();
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float sceneAspectRatio = windowSize.x / windowSize.y;
    Rectangle sceneRect = Rectangle{.x = 0.0f, .y = 0.0f, .width = 0.8f, .height=0.8f * sceneAspectRatio,};
    Rectangle modelUiRect = Rectangle{.x = sceneRect.width, .y = 0.0f, .width = 0.2f, .height=1.0f * sceneAspectRatio,};

    DefaultStuff defaultStuff;

    // Let's do save and load here
    AlSceneFileLoader sceneFileLoader("resources/scenes/hello-scene-01.json");
    AlSceneEditor sceneEditor(camera, sceneRect);

    // Load available models
    std::unordered_map<std::string, std::shared_ptr<RlModel>> internalModels{
            {"cube",   std::make_shared<RlModel>(RlModel{LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f))})},
            {"sphere", std::make_shared<RlModel>(RlModel{LoadModelFromMesh(GenMeshSphere(2.0f, 16, 16))})}
    };
    std::unordered_map<std::string, std::shared_ptr<RlModel>> loadedModels{};

    std::optional<AlSceneFileLoader_LoadedScene> loadedScene = sceneFileLoader.load(internalModels, loadedModels, defaultStuff.pbrShader);
    if (loadedScene.has_value()) {
        sceneEditor.objects.insert(sceneEditor.objects.end(), loadedScene->objects.begin(), loadedScene->objects.end());
    }

    AlModelUi modelUi(defaultStuff.unicodeFont, modelUiRect, "resources/KayKit_Prototype_Bits_1.0_FREE/Assets/gltf", defaultStuff.pbrShader);
    for (auto &[k, v]: internalModels) {
        modelUi.modelEntries.emplace_back(AlModelUi_Entry{k, true, v});
    }
    for (auto &[k, v]: loadedModels) {
        modelUi.modelEntries.emplace_back(AlModelUi_Entry{k, false, v});
    }

    float lightPower = 500.0f;
    std::vector<AlLight> lights{
            AlLight{.position = Vector3{-10.0f, 10.0f, -10.0f}, .colors = Vector3{lightPower, lightPower, lightPower}},
            AlLight{.position = Vector3{10.0f, 10.0f, -10.0f}, .colors = Vector3{lightPower, lightPower, lightPower}},
            AlLight{.position = Vector3{-10.0f, -10.0f, -10.0f}, .colors = Vector3{lightPower, lightPower, lightPower}},
            AlLight{.position = Vector3{10.0f, -10.0f, -10.0f}, .colors = Vector3{lightPower, lightPower, lightPower}},
    };

    float time = 0;
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        sceneEditor.handleInput();
        modelUi.handleInput();

        sceneEditor.tick(deltaTime);
        modelUi.tick(deltaTime);

        std::vector<AlObject> newObjects = modelUi.getSpawnedObjects();
        if (!newObjects.empty())
            sceneEditor.objects.insert(sceneEditor.objects.end(), newObjects.begin(), newObjects.end());

        if (IsKeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL)) {
            sceneFileLoader.save(sceneEditor.objects);
        }

        time += deltaTime;
        defaultStuff.pbrShader->passScene(camera, &lights);

        sceneEditor.renderToTexture();
        modelUi.renderToTexture();

        BeginDrawing();
        {
            ClearBackground(BLACK);

            DrawFPS(10, 10);

            sceneEditor.renderTexture();
            modelUi.renderTexture();
        }
        EndDrawing();

        DrawGrid(10, 1.0f);
    }

    CloseWindow();

    return errCode;
}
