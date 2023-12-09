//
// Created by alether on 10/1/23.
//

#include "scene_editor.h"

AlSceneEditor::AlSceneEditor(Camera3D camera, Rectangle normalizedSceneRect) :
        sceneViewport(normalizedSceneRect),
        gizmoViewport(normalizedSceneRect) {
    this->selectedObjectIndex = std::nullopt;
    this->camera = camera;
}

void AlSceneEditor::handleInput() {

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
    for (int i = 0; i < this->objects.size(); ++i) {
        AlObject *obj = &this->objects[i];
        for (int j = 0; j < obj->model->d.meshCount; ++j) {
            RayCollision rayCollision = GetRayCollisionMesh(ray, obj->model->d.meshes[j], obj->model->d.transform);
            if (rayCollision.hit && !this->selectedObjectIndex.has_value()) {
                this->selectedObjectIndex = std::make_optional(i);
                return true;
            }
        }
    }
    return false;
}

void AlSceneEditor::deselectObject() {
    this->selectedObjectIndex = std::nullopt;
}

void AlSceneEditor::render() {
    // Draw Gizmo
    this->gizmoViewport.beginRenderToTexture();
    {
        BeginMode3D(this->camera);
        {
            DrawFPS(10, 10);
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
                DrawModel(obj->model->d, Vector3Zero(), 1.0f, WHITE);
            }
            DrawGrid(10, 1.0f);
        }
        EndMode3D();
    }
    this->sceneViewport.endRenderToTexture();
}

void AlSceneEditor::renderRtt() {
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