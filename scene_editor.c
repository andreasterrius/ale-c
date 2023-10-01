//
// Created by alether on 10/1/23.
//

#include <stdio.h>
#include "scene_editor.h"

void alSceneEditorInit(AlSceneEditor *self) {
    self->selectedObject = NULL;
    self->objects = g_array_new(false, false, sizeof(AlObject));
    alGizmoInit(&self->gizmo);
    alArcCameraInputInit(&self->arcCameraInput);
}

void alSceneEditorHandleInput(AlSceneEditor *self, Camera *cam) {
    // try selecting an object
    // handle gizmo
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        bool clickedSomething = alSceneEditorSelectObject(self, cam);

        if (alGizmoTryHold(&self->gizmo, &self->selectedObject->transform, *cam)) {
            self->selectedObject->hasTransformChanged = true;
            alObjectTryRecalculate(self->selectedObject);
            clickedSomething = true;
        }
        if(!clickedSomething) {
            alSceneEditorDeselectObject(self);
        }
    } else {
        alGizmoRelease(&self->gizmo);
    }

    // handle camera
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        alArcCameraInputTryArcBall(&self->arcCameraInput, cam);
    } else {
        alArcCameraInputReleaseArcBall(&self->arcCameraInput);
    }
    alArcCameraInputZoomOut(&self->arcCameraInput, cam);
}

void alSceneEditorDeinit(AlSceneEditor *self) {
    alGizmoDeinit(&self->gizmo);
    for(int i = 0 ; i < self->objects->len; ++i){
        AlObject obj = g_array_index(self->objects, AlObject, i);
        alObjectDeinit(&obj);
    }
    g_array_free(self->objects, true);
}

bool alSceneEditorSelectObject(AlSceneEditor *self, Camera3D *cam) {
    Ray ray = GetMouseRay(GetMousePosition(), *cam);
    for(int i = 0; i < self->objects->len; ++i){
        AlObject *obj = &g_array_index(self->objects, AlObject, i);
        for (int j = 0; j < obj->model.meshCount; ++j){
            RayCollision rayCollision = GetRayCollisionMesh(ray, obj->model.meshes[j], obj->model.transform);
            if(rayCollision.hit) {
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

void alSceneEditorRender(AlSceneEditor *self) {
    for(int i = 0; i < self->objects->len; ++i){
        AlObject obj = g_array_index(self->objects, AlObject, i);
        DrawModel(obj.model, Vector3Zero(), 1.0f, WHITE);
    }
    alGizmoRender(self->gizmo);
}
