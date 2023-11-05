//
// Created by alether on 10/3/23.
//
#pragma once

#include<raylib.h>
#include<raymath.h>

/// Render To Texture
/// Will render on the original screen then put into a texture
typedef struct AlRtt {
    RenderTexture2D renderTexture2D;

    // Where and how big we want the texture to be rendered
    Rectangle actualDest;
    Rectangle normalizedDest;
    bool needRecalculateActualDest;
} AlRtt;

/// normalizedDest=NULL means it will always stretch to screen size
/// normalizedDest!=NULL means it will be copied and no reference will be made to it
void alRttInit(AlRtt *self, Rectangle normalizedDest);

void alRttDeinit(AlRtt *self);

void alRttBeginRenderToTexture(AlRtt self);

void alRttEndRenderToTexture(AlRtt self);

void alRttRenderTexture(AlRtt self);

void alRttTryRecalculateRect(AlRtt *self);

/// Get mouse position IN the coordinates of the texture (dest.x dest.y as 0.0)
Vector2 alRttGetMousePosition(AlRtt self);

/// Shoot a ray with the given mouse position
Ray alRttGetMouseRay(AlRtt self, Camera3D camera);
