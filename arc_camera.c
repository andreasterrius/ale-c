#include "arc_camera.h"

AlArcCameraInput alArcCameraInputInit(AlArcCameraInput *self) {
    self->isHeld = false,
            self->lastFrameMousePos = Vector2Zero();
}

void alArcCameraInputTryArcBall(AlArcCameraInput *self, Camera3D *camera) {
    if(!self->isHeld) {
        self->lastFrameMousePos = GetMousePosition();
        self->isHeld = true;
        return;
    }
    // isHeld: true
    Vector2 currFrameMousePos = GetMousePosition();
    Vector2 delta = Vector2Subtract(currFrameMousePos, self->lastFrameMousePos);
    self->lastFrameMousePos = currFrameMousePos;

    Vector3 camForward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 camUp = camera->up;
    Vector3 camRight = Vector3CrossProduct(camForward, camUp);

    Quaternion goUp = QuaternionFromAxisAngle(camRight, DEG2RAD*delta.y * -1);
    Quaternion goRight = QuaternionFromAxisAngle(camUp, DEG2RAD*delta.x * -1);
    Quaternion combined = QuaternionMultiply(goUp, goRight);

    Vector3 pivotToPosition  = Vector3Subtract(camera->position, camera->target);
    camera->position = Vector3RotateByQuaternion(pivotToPosition, combined);
}

void alArcCameraInputReleaseArcBall(AlArcCameraInput *self) {
    self->isHeld = false;
}

void alArcCameraInputZoomOut(AlArcCameraInput *self, Camera3D *camera) {
    float mouseWheelMove = GetMouseWheelMove();
    Vector3 forwardDir = Vector3Subtract(camera->target, camera->position);
    camera->position = Vector3Add(camera->position, (Vector3){
        .x = forwardDir.x * mouseWheelMove * 0.1f,
        .y = forwardDir.y * mouseWheelMove * 0.1f,
        .z = forwardDir.z * mouseWheelMove * 0.1f,
    });
}
