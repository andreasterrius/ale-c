//
// Created by alether on 10/1/23.
//

#ifndef HELLO_C_SCENE_EDITOR_H
#define HELLO_C_SCENE_EDITOR_H

#include"gizmo.h"
#include"object.h"
#include"arc_camera.h"
#include"rtt.h"
#include"types.h"
#include<vector>
#include<optional>

class AlSceneEditor {
    Camera3D camera;

    // currently selected object
    std::optional<i32> selectedObjectIndex;

    // translation, rotation, and scaling gizmo
    AlGizmo gizmo;
    AlRtt gizmoViewport;

    // Scene viewport
    AlRtt sceneViewport;

    // camera input state
    AlArcCameraInput arcCameraInput;

public:
    // all objects currently in the scene
    std::vector<AlObject> objects;

    AlSceneEditor(Camera3D camera, Rectangle normalizedRect);

    void tick(float deltaTime);

    void renderToTexture();

    void renderTexture();

    void handleInput();

    bool selectObject();

    void deselectObject();
};


#endif //HELLO_C_SCENE_EDITOR_H
