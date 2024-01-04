//
// Created by alether on 10/1/23.
//

#include "scene_editor.h"
#include "rlmath.h"

AlSceneEditor::AlSceneEditor(Camera3D camera, Rectangle normalizedSceneRect) :
        sceneViewport(normalizedSceneRect),
        gizmoViewport(normalizedSceneRect) {
    this->selectedObjectIndex = std::nullopt;
    this->camera = camera;
}

void AlSceneEditor::handleInput() {

    // only handle when mouse is above the render target rect
    // we check this on world position
    Vector2 mousePos = GetMousePosition();
    if (!IsPointInsideRect(this->sceneViewport.actualDest, mousePos)) {
        return;
    }

    {
        /// SECTION: Right click, camera rotation
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            this->arcCameraInput.tryArcBall(&this->camera);
        } else {
            this->arcCameraInput.releaseArcBall();
        }
        this->arcCameraInput.zoomOut(&this->camera);
    }

    {
        /// SECTION: Gizmo + Mouse Click
        bool clickedSomething = false;  // if we're clicking anything, this should be true.
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clickedSomething = clickedSomething || this->selectObject();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && this->selectedObjectIndex.has_value()) {
            AlObject *selectedObject = &this->objects[selectedObjectIndex.value()];
            clickedSomething = clickedSomething || this->gizmo.tryHold(&selectedObject->transform,
                                                                       this->sceneViewport.getScaledLocalMousePos(),
                                                                       this->camera);
            selectedObject->hasTransformChanged = true;
        } else {
            this->gizmo.release();
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !clickedSomething) {
            this->deselectObject();
            this->gizmo.hide();
        }

        if (IsKeyDown(KEY_W)) {
            this->gizmo.gizmoType = AleGizmo_Type::Translate;
        } else if (IsKeyDown(KEY_R)) {
            this->gizmo.gizmoType = AleGizmo_Type::Rotate;
        } else if (IsKeyDown(KEY_E)) {
            this->gizmo.gizmoType = AleGizmo_Type::Scale;
        }
    }
}

bool AlSceneEditor::selectObject() {
    Ray ray = this->sceneViewport.getMouseRay(this->camera);
    float currHitDist = std::numeric_limits<float>::max();
    bool hitSomething = false;
    for (int i = 0; i < this->objects.size(); ++i) {
        AlObject *obj = &this->objects[i];
        obj->model->d.transform = obj->transformMat;
        for (int j = 0; j < obj->model->d.meshCount; ++j) {
            RayCollision rayCollision = GetRayCollisionMesh(ray, obj->model->d.meshes[j], obj->model->d.transform);
            if (rayCollision.hit) {
                float dist = Vector3Distance(this->camera.position, rayCollision.point);
                if (dist > 0.0f && dist < currHitDist) {
                    this->selectedObjectIndex = std::make_optional(i);
                    hitSomething = true;
                    currHitDist = dist;
                }
            }
        }
    }
    return hitSomething;
}

void AlSceneEditor::deselectObject() {
    this->selectedObjectIndex = std::nullopt;
}

void AlSceneEditor::renderToTexture() {
    // Draw Gizmo
    this->gizmoViewport.beginRenderToTexture();
    {
        BeginMode3D(this->camera);
        {
            DrawText("scene editor demo!", 100, 100, 20, YELLOW);

            this->gizmo.render();
        }
        EndMode3D();
    }
    this->gizmoViewport.endRenderToTexture();

    // Draw Scene
    this->sceneViewport.beginRenderToTexture();
    {
        ClearBackground(SKYBLUE);

        // Draw objects
        BeginMode3D(this->camera);
        {
            for (int i = 0; i < this->objects.size(); ++i) {
                AlObject *obj = &this->objects[i];
                obj->model->d.transform = obj->transformMat;
                DrawModel(obj->model->d, Vector3Zero(), 1.0f, WHITE);
            }
            DrawGrid(10, 1.0f);
        }
        EndMode3D();
    }
    this->sceneViewport.endRenderToTexture();
}

void AlSceneEditor::renderTexture() {
    // Draw everything to screen
    this->sceneViewport.renderTexture();
    this->gizmoViewport.renderTexture();
}

void AlSceneEditor::tick(float deltaTime) {
    for (int i = 0; i < this->objects.size(); ++i) {
        AlObject *object = &this->objects[i];
        object->tryRecalculate();
    }
    this->gizmoViewport.tryRecalculateRect();
    this->sceneViewport.tryRecalculateRect();
}