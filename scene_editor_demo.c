#include <raylib.h>
#include <raymath.h>
#include "object.h"
#include "scene_editor.h"
#include "src/ui/model_ui.h"
#include "unicode_font.h"

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
    Rectangle sceneRect = (Rectangle){.height=0.8f * sceneAspectRatio, .width=0.8f, .x=0.0f, .y=0.0f};
    Rectangle modelUiRect = (Rectangle){.height=1.0f * sceneAspectRatio, .width=0.2f, .x=sceneRect.width, .y=0.0f};

    // Load available models
    AlSceneEditor sceneEditor;
    alSceneEditorInit(&sceneEditor, camera, sceneRect);

    AlArray ranges;
    alArrayInit(&ranges, sizeof(AlUnicodeFontRange), 4);

    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 0, .end=255}));
//    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 12352, .end=12543}));
//    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 19968, .end=40959}));
//    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 44032, .end=55215}));

    AlUnicodeFont unicodeFont;
    if(!alUnicodeFontInit(&unicodeFont, "resources/font/NotoSansCJKjp-VF.ttf", 24, ranges)){
        errCode = -1;
        goto cleanupUnicode;
    }

    AlModelUi modelUi;
    alModelUiInit(&modelUi, &unicodeFont, modelUiRect);

    AlModelUiEntry alModelUiEntry1;
    alModelUiEntry1.name = "someName";
    AlModelUiEntry alModelUiEntry2;
    alModelUiEntry2.name = "anotherName123\nbeedfeg";

    alArrayPush(&modelUi.modelList, &alModelUiEntry1);
    alArrayPush(&modelUi.modelList, &alModelUiEntry2);

    AlObject object;
    object.transform = (Transform) {
            .translation = Vector3Zero(),
            .rotation = QuaternionIdentity(),
            .scale = Vector3One(),
    };
    object.model = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 1.0f));
    object.hasTransformChanged = true;

    AlObject anotherObject;
    anotherObject.transform = (Transform) {
            .translation = (Vector3) {.x=3, .y=3, .z=3},
            .scale = Vector3One(),
            .rotation = QuaternionIdentity()
    };
    anotherObject.model = LoadModelFromMesh(GenMeshSphere(2.0f, 16, 16));
    anotherObject.hasTransformChanged = true;

    // let's add this manually for now
    alArrayPush(&sceneEditor.objects, &object);
    alArrayPush(&sceneEditor.objects, &anotherObject);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        alSceneEditorHandleInput(&sceneEditor);
        alSceneEditorTick(&sceneEditor, deltaTime);
        alModelUiTick(&modelUi);

        alSceneEditorRender(&sceneEditor);
        alModelUiRender(&modelUi);

        {
            BeginDrawing();

            ClearBackground(SKYBLUE);
            alSceneEditorRenderRtt(&sceneEditor);
            alModelUiRenderRtt(&modelUi);

            EndDrawing();
        }

        DrawGrid(10, 1.0f);
    }

    alModelUiDeinit(&modelUi);
cleanupUnicode:
    alUnicodeFontDeinit(&unicodeFont);
    alArrayDeinit(&ranges);
    alSceneEditorDeinit(&sceneEditor);
    CloseWindow();

    return errCode;
}
