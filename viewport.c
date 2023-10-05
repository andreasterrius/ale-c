//
// Created by alether on 10/3/23.
//

#include "viewport.h"

void alViewportDeinit(AlViewport *self) {
    UnloadRenderTexture(self->renderTexture2D);
}

void alViewportInit(AlViewport *self, Vector2 origin, Vector2 size, Vector2 *parentSize) {
    self->origin = origin;
    self->size = size;
    self->parentSize = parentSize;
    self->renderTexture2D = LoadRenderTexture(600, 400);
}

void alViewportBeginRender(const AlViewport self) {
    BeginTextureMode(self.renderTexture2D);
}

void alViewportEndRender(const AlViewport self) {
    EndTextureMode();
}

void alViewportRenderToScreen(const AlViewport self) {
    ClearBackground(RAYWHITE);

    DrawTexturePro(self.renderTexture2D.texture,
                   (Rectangle) {0.0f, 0.0f, (float) self.renderTexture2D.texture.width, -self.renderTexture2D.texture.height},
                   (Rectangle) {0.0f, 0.0f, (float) self.renderTexture2D.texture.width, self.renderTexture2D.texture.height},
                   self.origin, 0.0f, WHITE);
}

Vector2 alViewportGetMousePosition(AlViewport self) {
    Vector2 mousePos = GetMousePosition();
    Vector2 resultPos = (Vector2){
            .x = self.parentSize->x / self.size.x * (self.origin.x + mousePos.x),
            .y = self.parentSize->y / self.size.y * (self.origin.y + mousePos.y)
    };
    return resultPos;
}

Ray alViewportGetMouseRay(const AlViewport self, Camera3D camera) {
    Vector2 mousePos = alViewportGetMousePosition(self);
    Ray ray = GetMouseRay(mousePos, camera);
    return ray;
}
