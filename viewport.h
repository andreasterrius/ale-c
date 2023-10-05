//
// Created by alether on 10/3/23.
//
#pragma once

#include<raylib.h>
#include<raymath.h>

typedef struct AlViewport {
    // Weak reference to original window size
    // For now we always assume parent origin (0,0)
    Vector2 *parentSize;

    RenderTexture2D renderTexture2D;
    Vector2 origin;
    Vector2 size;

} AlViewport;

void alViewportInit(AlViewport *self, Vector2 origin, Vector2 size, Vector2 *parentSize);

void alViewportDeinit(AlViewport *self);

void alViewportBeginRender(AlViewport self);

void alViewportEndRender(AlViewport self);

void alViewportRenderToScreen(AlViewport self);

Vector2 alViewportGetMousePosition(AlViewport self);

Ray alViewportGetMouseRay(AlViewport self, Camera3D camera);
