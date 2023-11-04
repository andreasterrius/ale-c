//
// Created by alether on 10/3/23.
//

#include <stddef.h>
#include <stdio.h>
#include "rtt.h"

void alRttDeinit(AlRtt *self) {
    UnloadRenderTexture(self->renderTexture2D);
}

void alRttInit(AlRtt *self, Rectangle *dest) {
    if(dest == NULL) {
        self->dest = (Rectangle) {
            .width=GetScreenWidth(),
            .height=GetScreenHeight(),
            .x=0,
            .y=0,
        };
        self->stretchToScreenSize = true;
    } else {
        self->dest = *dest;
        self->stretchToScreenSize = false;
    }
    self->renderTexture2D = LoadRenderTexture(self->dest.width, self->dest.height);
}

void alRttBeginRenderToTexture(AlRtt *self) {

    // TODO: Move this somewhere, this function should not change anything
    // just stretch this for now
    if(self->stretchToScreenSize && IsWindowResized()) {
        self->dest.width = GetScreenWidth();
        self->dest.height = GetScreenHeight();
        UnloadRenderTexture(self->renderTexture2D);
        self->renderTexture2D = LoadRenderTexture(self->dest.width, self->dest.height);
    }

    BeginTextureMode(self->renderTexture2D);
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
                   self.dest,
                   Vector2Zero(), 0.0f, WHITE);

    EndBlendMode();
}

Vector2 alRttGetMousePosition(AlRtt self) {
    Vector2 mousePos = GetMousePosition();
    Vector2 resultPos = (Vector2) {
            .x = (float) GetScreenWidth() / self.dest.width * (mousePos.x - self.dest.x),
            .y = (float) GetScreenHeight() / self.dest.height * (mousePos.y - self.dest.y)
    };
    return resultPos;
}

Ray alRttGetMouseRay(AlRtt self, Camera3D camera) {
    Vector2 mousePos = alRttGetMousePosition(self);
    Ray ray = GetMouseRay(mousePos, camera);
    return ray;
}
