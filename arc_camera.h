//
// Created by alether on 9/28/23.
//

#ifndef HELLO_C_ARC_CAMERA_H
#define HELLO_C_ARC_CAMERA_H

#include<raylib.h>
#include<raymath.h>

typedef struct AlArcCameraInput {
    bool isStarted;
    Vector2 lastFrameMousePos;
} AlArcCameraInput;

// Doesn't have deinit
AlArcCameraInput alArcCameraInputInit() {
    return (AlArcCameraInput){
        .isStarted = false,
        .lastFrameMousePos = Vector2Zero(),
    };
}

/// Try to hold and rotate the camera
/// Can release with alArcCameraInputRelease
void alArcCameraInputTryHold(AlArcCameraInput *self) {
    if(!self->isStarted) {
        self->lastFrameMousePos = GetMousePosition();
        return;
    }
}

void alArcCameraInputRelease(AlArcCameraInput *self) {

}


#endif //HELLO_C_ARC_CAMERA_H
