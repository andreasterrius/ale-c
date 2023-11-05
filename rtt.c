//
// Created by alether on 10/3/23.
//

#include <stddef.h>
#include <stdio.h>
#include "rtt.h"

void alRttDeinit(AlRtt *self) {
    UnloadRenderTexture(self->renderTexture2D);
}

void alRttInit(AlRtt *self, Rectangle normalizedDest) {
    self->normalizedDest = normalizedDest;
    self->actualDest = (Rectangle) {
            .width=GetScreenWidth() * self->normalizedDest.width,
            .height=GetScreenHeight() * self->normalizedDest.height,
            .x=GetScreenWidth() * self->normalizedDest.x,
            .y=GetScreenHeight() * self->normalizedDest.y,
    };
    self->renderTexture2D = LoadRenderTexture(self->actualDest.width, self->actualDest.height);
    self->needRecalculateActualDest = false;
}

void alRttBeginRenderToTexture(AlRtt self) {
    BeginTextureMode(self.renderTexture2D);
    ClearBackground(ColorAlpha(RED, 0.0f));
}

void alRttEndRenderToTexture(AlRtt self) {
    EndTextureMode();
}

void alRttRenderTexture(AlRtt self) {
    BeginBlendMode(BLEND_ALPHA);


    // This will not be scaled if screen is resized
    // We have to change self.rect if we want the texture to be stretched according to raylib's fill
    // We have to change the source if we want the texture to be properly sized
    DrawTexturePro(self.renderTexture2D.texture,
                   (Rectangle) {0.0f, 0.0f, self.renderTexture2D.texture.width, -self.renderTexture2D.texture.height},
                   self.actualDest,
                   Vector2Zero(), 0.0f, WHITE);

    EndBlendMode();
}

Vector2 alRttGetMousePosition(AlRtt self) {
    Vector2 mousePos = GetMousePosition();
    Vector2 resultPos = (Vector2) {
            .x = (float) GetScreenWidth() / self.actualDest.width * (mousePos.x - self.actualDest.x),
            .y = (float) GetScreenHeight() / self.actualDest.height * (mousePos.y - self.actualDest.y)
    };
    return resultPos;
}

Ray alRttGetMouseRay(AlRtt self, Camera3D camera) {
    Vector2 mousePos = alRttGetMousePosition(self);
    Ray ray = GetMouseRay(mousePos, camera);
    return ray;
}

void alRttTryRecalculateRect(AlRtt *self) {
    if(self->needRecalculateActualDest || IsWindowResized()) {
        self->actualDest = (Rectangle) {
                .width=GetScreenWidth() * self->normalizedDest.width,
                .height=GetScreenHeight() * self->normalizedDest.height,
                .x=GetScreenWidth() * self->normalizedDest.x,
                .y=GetScreenHeight() * self->normalizedDest.y,
        };
        UnloadRenderTexture(self->renderTexture2D);
        self->renderTexture2D = LoadRenderTexture(self->actualDest.width, self->actualDest.height);
        self->needRecalculateActualDest = false;
    }
}
