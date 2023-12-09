//
// Created by alether on 10/3/23.
//
#pragma once

#include<raylib.h>
#include<raymath.h>
#include<memory>
#include"rldata.h"

/// Render To Texture
/// Will render on the original screen then put into a texture
class AlRtt {
    RlRenderTexture2D renderTexture2D;

public:
    // Where and how big we want the texture to be rendered
    Rectangle normalizedDest;

    /// Can be calculated automatically on tryRecalculateRect()
    Rectangle actualDest;
    bool needRecalculateActualDest;

public:
    /// normalizedDest=NULL means it will always stretch to screen size
    /// normalizedDest!=NULL means it will be copied and no reference will be made to it
    explicit AlRtt(Rectangle normalizedDest);

    void beginRenderToTexture();

    void endRenderToTexture();

    void renderTexture();

    /// If there's a window/layout resize, we need to call this
    bool tryRecalculateRect();

    /// Get mouse position IN the coordinates of the texture (dest.x dest.y as 0.0)
    Vector2 getLocalMousePos();

    /// Get mouse position IN the coordinates of the texture (dest.x dest.y as 0.0)
    /// SCALED to screen size, usually used by GetMouseRay()
    Vector2 getScaledLocalMousePos();

    Vector2 getLocalToWorld(Vector2 localPos);

    Vector2 getWorldToLocal(Vector2 worldPos);

    /// Shoot a ray with the given mouse position
    Ray getMouseRay(Camera3D camera);
};

