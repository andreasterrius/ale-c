//
// Created by alether on 10/1/23.
//

#ifndef HELLO_C_SCENE_EDITOR_H
#define HELLO_C_SCENE_EDITOR_H

#include"gizmo.h"
#include"object.h"
#include"arc_camera.h"
#include"array.h"
#include"rtt.h"
#include"types.h"

typedef struct AlSceneEditor {
    // all objects currently in the scene
    AlArray objects; //AlAray<AlObject>
    Camera3D camera;

    // currently selected object
    AlObject *selectedObject;

    // translation, rotation, and scaling gizmo
    AlGizmo gizmo;
    AlRtt gizmoViewport;

    // Scene viewport
    AlRtt sceneViewport;

    // UI viewport

    // camera input state
    AlArcCameraInput arcCameraInput;

} AlSceneEditor;

void alSceneEditorInit(AlSceneEditor *self, Camera3D camera);

void alSceneEditorDeinit(AlSceneEditor *self);

void alSceneEditorTick(AlSceneEditor *self, float deltaTime);

void alSceneEditorRender(const AlSceneEditor *self);

void alSceneEditorHandleInput(AlSceneEditor *self);

static bool alSceneEditorSelectObject(AlSceneEditor *self);

static void alSceneEditorDeselectObject(AlSceneEditor *self);

#endif //HELLO_C_SCENE_EDITOR_H
