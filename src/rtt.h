//
// Created by alether on 10/3/23.
//
#pragma once

#include<raylib.h>
#include<raymath.h>
#include<memory>

/// Render To Texture
/// Will render on the original screen then put into a texture
class AlRtt {
    std::unique_ptr<RenderTexture2D> renderTexture2D;

    // Where and how big we want the texture to be rendered
    Rectangle actualDest;
    Rectangle normalizedDest;
    bool needRecalculateActualDest;

public:
    // disable copy constructor.
    AlRtt (const AlRtt&) = delete;
    AlRtt& operator= (const AlRtt&) = delete;

    // enable move ctor
    AlRtt (AlRtt&&) = default;
    AlRtt& operator= (const AlRtt&&) = default;

public:
    /// normalizedDest=NULL means it will always stretch to screen size
    /// normalizedDest!=NULL means it will be copied and no reference will be made to it
    AlRtt(Rectangle normalizedDest);

    ~AlRtt();

    void beginRenderToTexture();

    void endRenderToTexture();

    void renderTexture();

    void tryRecalculateRect();

    /// Get mouse position IN the coordinates of the texture (dest.x dest.y as 0.0)
    Vector2 getMousePosition();

    /// Shoot a ray with the given mouse position
    Ray getMouseRay(Camera3D camera);
};

