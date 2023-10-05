//
// Created by alether on 10/1/23.
//

#ifndef HELLO_C_SCENE_EDITOR_H
#define HELLO_C_SCENE_EDITOR_H

#include<glib.h>
#include"gizmo.h"
#include"object.h"
#include"arc_camera.h"
#include"viewport.h"

typedef struct AlSceneEditor {
    // all objects currently in the scene
    GArray *objects; //AlObject

    // currently selected object
    AlObject *selectedObject;

    // translation, rotation, and scaling gizmo
    AlGizmo gizmo;
    AlViewport gizmoViewport;

    // camera input state
    AlArcCameraInput arcCameraInput;

} AlSceneEditor;

void alSceneEditorInit(AlSceneEditor *self, Vector2 *windowSize);

void alSceneEditorDeinit(AlSceneEditor *self);

void alSceneEditorRender(AlSceneEditor *self);

void alSceneEditorHandleInput(AlSceneEditor *self, Camera *cam);

static bool alSceneEditorSelectObject(AlSceneEditor *self, Camera3D *cam);

static void alSceneEditorDeselectObject(AlSceneEditor *self);

#endif //HELLO_C_SCENE_EDITOR_H
