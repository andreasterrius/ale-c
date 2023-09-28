//
// Created by alether on 9/28/23.
//

#ifndef HELLO_C_ARC_CAMERA_H
#define HELLO_C_ARC_CAMERA_H

#include<raylib.h>
#include<raymath.h>

typedef struct AlArcCameraInput {
    bool isHeld; //whether the mouse is held or not
    Vector2 lastFrameMousePos;
} AlArcCameraInput;

// Doesn't have deinit
AlArcCameraInput alArcCameraInputInit(AlArcCameraInput *self);

/// Try to zoom out
void alArcCameraInputZoomOut(AlArcCameraInput *self, Camera3D *camera);

/// Try to hold and rotate the camera
/// Can release with alArcCameraInputReleaseArcBall
void alArcCameraInputTryArcBall(AlArcCameraInput *self, Camera3D *camera);

void alArcCameraInputReleaseArcBall(AlArcCameraInput *self);


#endif //HELLO_C_ARC_CAMERA_H
