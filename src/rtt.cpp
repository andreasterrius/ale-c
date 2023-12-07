//
// Created by alether on 10/3/23.
//

#include "rtt.h"


AlRtt::AlRtt(Rectangle normalizedDest) {
    this->normalizedDest = normalizedDest;
    this->actualDest = (Rectangle) {
            .width=GetScreenWidth() * this->normalizedDest.width,
            .height=GetScreenHeight() * this->normalizedDest.height,
            .x=GetScreenWidth() * this->normalizedDest.x,
            .y=GetScreenHeight() * this->normalizedDest.y,
    };
    this->renderTexture2D = RlRenderTexture2D{LoadRenderTexture(this->actualDest.width, this->actualDest.height)};
    this->needRecalculateActualDest = false;
}

void AlRtt::beginRenderToTexture() {
    BeginTextureMode(this->renderTexture2D.d);
    ClearBackground(ColorAlpha(RED, 0.0f));
}

void AlRtt::endRenderToTexture() {
    EndTextureMode();
}

void AlRtt::renderTexture() {
    BeginBlendMode(BLEND_ALPHA);


    // This will not be scaled if screen is resized
    // We have to change this->rect if we want the texture to be stretched according to raylib's fill
    // We have to change the source if we want the texture to be properly sized
    DrawTexturePro(this->renderTexture2D.d.texture,
                   (Rectangle) {0.0f, 0.0f,
                                static_cast<float>(this->renderTexture2D.d.texture.width),
                                static_cast<float>(-this->renderTexture2D.d.texture.height)},
                   this->actualDest,
                   Vector2Zero(), 0.0f, WHITE);

    EndBlendMode();
}

Vector2 AlRtt::getMousePosition() {
    Vector2 mousePos = GetMousePosition();
    Vector2 resultPos = (Vector2) {
            .x = (float) GetScreenWidth() / this->actualDest.width * (mousePos.x - this->actualDest.x),
            .y = (float) GetScreenHeight() / this->actualDest.height * (mousePos.y - this->actualDest.y)
    };
    return resultPos;
}

Ray AlRtt::getMouseRay(Camera3D camera) {
    Vector2 mousePos = this->getMousePosition();
    Ray ray = GetMouseRay(mousePos, camera);
    return ray;
}

bool AlRtt::tryRecalculateRect() {
    if (this->needRecalculateActualDest || IsWindowResized()) {
        this->actualDest = (Rectangle) {
                .width=GetScreenWidth() * this->normalizedDest.width,
                .height=GetScreenHeight() * this->normalizedDest.height,
                .x=GetScreenWidth() * this->normalizedDest.x,
                .y=GetScreenHeight() * this->normalizedDest.y,
        };
        UnloadRenderTexture(this->renderTexture2D.d);
        this->renderTexture2D = LoadRenderTexture(this->actualDest.width, this->actualDest.height);
        this->needRecalculateActualDest = false;
        return true;
    }
    return false;
}
