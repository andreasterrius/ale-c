//
// Created by alether on 10/1/23.
//

#include <stdio.h>
#include "scene_editor.h"
#include "defer.h"

void alSceneEditorInit(AlSceneEditor *self, Camera3D camera, Vector2 *windowSize) {
    self->selectedObject = NULL;
    kv_init(self->objects);
    self->camera = camera;
    alGizmoInit(&self->gizmo);
    alRttInit(&self->gizmoViewport, NULL);
    alArcCameraInputInit(&self->arcCameraInput);
}

void alSceneEditorHandleInput(AlSceneEditor *self) {
    // try selecting an object
    // handle gizmo
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        bool clickedSomething = alSceneEditorSelectObject(self);

        if (alGizmoTryHold(&self->gizmo, &self->selectedObject->transform, self->camera)) {
            self->selectedObject->hasTransformChanged = true;
            alObjectTryRecalculate(self->selectedObject);
            clickedSomething = true;
        }
        if (!clickedSomething) {
            alSceneEditorDeselectObject(self);
        }
    } else {
        alGizmoRelease(&self->gizmo);
    }

    // handle camera
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        alArcCameraInputTryArcBall(&self->arcCameraInput, &self->camera);
    } else {
        alArcCameraInputReleaseArcBall(&self->arcCameraInput);
    }
    alArcCameraInputZoomOut(&self->arcCameraInput, &self->camera);
}

void alSceneEditorDeinit(AlSceneEditor *self) {
    alGizmoDeinit(&self->gizmo);
    alRttDeinit(&self->gizmoViewport);
    kv_destroy(self->objects);
}

bool alSceneEditorSelectObject(AlSceneEditor *self) {
    Ray ray = GetMouseRay(GetMousePosition(), self->camera);
    for (int i = 0; i < kv_size(self->objects); ++i) {
        AlObject *obj = &kv_A(self->objects, i);
        for (int j = 0; j < obj->model.meshCount; ++j) {
            RayCollision rayCollision = GetRayCollisionMesh(ray, obj->model.meshes[j], obj->model.transform);
            if (rayCollision.hit) {
                self->selectedObject = obj; //dangerous? anyway selectedObject is non owning
                return true;
            }
        }
    }
    return false;
}

void alSceneEditorDeselectObject(AlSceneEditor *self) {
    self->selectedObject = NULL;
}

void alSceneEditorRender(const AlSceneEditor *self) {

    // Draw Gizmo
    {
        alRttBeginRender(&self->gizmoViewport);
        defer { alRttEndRender(self->gizmoViewport); };

        DrawFPS(10, 10);
        DrawText("scene editor demo!", 100, 100, 20, YELLOW);

        BeginMode3D(self->camera);
        defer{ EndMode3D(); };

        alGizmoRender(self->gizmo);
    }

    // Draw everything to screen
    {
        BeginDrawing();
        defer{ EndDrawing(); };

        ClearBackground(SKYBLUE);

        // Draw objects
        {
            BeginMode3D(self->camera);
            defer{ EndMode3D(); };
            for (int i = 0; i < kv_size(self->objects); ++i) {
                AlObject obj = kv_A(self->objects, i);
                DrawModel(obj.model, Vector3Zero(), 1.0f, WHITE);
            }
            DrawGrid(10, 1.0f);
        }

        // Draw textures
        alRttRenderToScreen(self->gizmoViewport);
    }

}
