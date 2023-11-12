//
// Created by alether on 10/1/23.
//

#include <stdio.h>
#include "scene_editor.h"

void alSceneEditorInit(AlSceneEditor *self, Camera3D camera, Rectangle normalizedSceneRect) {
    self->selectedObject = NULL;
    self->camera = camera;
    alArrayInit(&self->objects, sizeof(AlObject), 32);
    alGizmoInit(&self->gizmo);
    alArcCameraInputInit(&self->arcCameraInput);

    alRttInit(&self->sceneViewport, normalizedSceneRect);
    alRttInit(&self->gizmoViewport, normalizedSceneRect);
}

void alSceneEditorHandleInput(AlSceneEditor *self) {

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        bool clickedSomething = alSceneEditorSelectObject(self);

        if (alGizmoTryHold(&self->gizmo, &self->selectedObject->transform, alRttGetMousePosition(self->sceneViewport),
                           self->camera)) {
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
    alRttDeinit(&self->sceneViewport);
    for (int i = 0; i < alArraySize(self->objects); ++i) {
        AlObject *object = (AlObject *) alArrayGet(self->objects, i);
        alObjectDeinit(object);
    }
    alArrayDeinit(&self->objects);
}

bool alSceneEditorSelectObject(AlSceneEditor *self) {
    Ray ray = alRttGetMouseRay(self->sceneViewport, self->camera);
    for (int i = 0; i < alArraySize(self->objects); ++i) {
        AlObject *obj = alArrayGet(self->objects, i);
        for (int j = 0; j < obj->model.meshCount; ++j) {
            RayCollision rayCollision = GetRayCollisionMesh(ray, obj->model.meshes[j], obj->model.transform);
            if (rayCollision.hit && self->selectedObject == NULL) {
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
    alRttBeginRenderToTexture(self->gizmoViewport);
    {
        BeginMode3D(self->camera);
        {
            DrawFPS(10, 10);
            DrawText("scene editor demo!", 100, 100, 20, YELLOW);

            alGizmoRender(self->gizmo);
        }
        EndMode3D();
    }
    alRttEndRenderToTexture(self->gizmoViewport);

    // Draw Scene
    alRttBeginRenderToTexture(self->sceneViewport);
    {
        ClearBackground(SKYBLUE);

        // Draw objects
        BeginMode3D(self->camera);
        {
            for (int i = 0; i < alArraySize(self->objects); ++i) {
                AlObject *obj = alArrayGet(self->objects, i);
                DrawModel(obj->model, Vector3Zero(), 1.0f, WHITE);
            }
            DrawGrid(10, 1.0f);
        }
        EndMode3D();
    }
    alRttEndRenderToTexture(self->sceneViewport);
}

void alSceneEditorRenderRtt(const AlSceneEditor *self) {
    // Draw everything to screen
    alRttRenderTexture(self->sceneViewport);
    alRttRenderTexture(self->gizmoViewport);
}

void alSceneEditorTick(AlSceneEditor *self, float deltaTime) {
    for (int i = 0; i < alArraySize(self->objects); ++i) {
        AlObject *object = (AlObject *) alArrayGet(self->objects, i);
        alObjectTryRecalculate(object);
    }
    alRttTryRecalculateRect(&self->gizmoViewport);
    alRttTryRecalculateRect(&self->sceneViewport);
}
