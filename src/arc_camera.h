//
// Created by alether on 9/28/23.
//

#ifndef HELLO_C_ARC_CAMERA_H
#define HELLO_C_ARC_CAMERA_H

#include<raylib.h>
#include<raymath.h>

class AlArcCameraInput {
    bool isHeld; //whether the mouse is held or not
    Vector2 lastFrameMousePos;

public:
    AlArcCameraInput();

    void zoomOut(Camera3D *camera);

    void tryArcBall(Camera3D *camera);

    void releaseArcBall();
};

#endif //HELLO_C_ARC_CAMERA_H
