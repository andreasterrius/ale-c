#include <raylib.h>
#include <raymath.h>
#include "../../rlmath.h"
#include "../../gizmo.h"
#include "../../object.h"
#include <memory>

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

    AlObject object(TransformOrigin(),
                    std::make_shared<RlModel>(LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 1.0f))));
    AlGizmo gizmo;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(10, 10);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (gizmo.tryHold(&object.transform, GetMousePosition(), camera)) {
                object.hasTransformChanged = true;
            }
        } else {
            gizmo.release();
        }

        DrawText("gizmo demo!", 100, 100, 20, YELLOW);
        BeginMode3D(camera);

        object.tryRecalculate();
        gizmo.render();

        DrawModel(object.model.get()->d, Vector3Zero(), 1.0f, WHITE);
        DrawGrid(10, 1.0f);

        EndMode3D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
