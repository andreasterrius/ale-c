//
// Created by Alether on 11/5/2023.
//
#include "model_ui.h"

void alModelUiInit(AlModelUi *self, AlUnicodeFont *unicodeFont, Rectangle normalizedDest) {
    self->unicodeFont = unicodeFont;
    alArrayInit(&self->modelList, sizeof(AlModelUiEntry), 8);
    alRttInit(&self->view, normalizedDest);
}

void alModelUiDeinit(AlModelUi *self) {
    alArrayDeinit(&self->modelList);
    alRttDeinit(&self->view);
}

void alModelUiRender(AlModelUi *self) {
    Rectangle rect = (Rectangle){
            .width = GetScreenWidth(),
            .height = GetScreenHeight(),
            .x = 0,
            .y = 0
    };

    alRttTryRecalculateRect(&self->view);

    // Draw Texts
    {
        alRttBeginRenderToTexture(self->view);
        defer { alRttEndRenderToTexture(self->view); };

        DrawRectangle(0, 0, self->view.actualDest.width, self->view.actualDest.height, RED);

        for(int i = 0; i < alArraySize(self->modelList); ++i) {
            AlModelUiEntry *entry = (AlModelUiEntry*) alArrayGet(self->modelList, i);
            rect.y += 100;

            alUnicodeFontRenderBoxed(self->unicodeFont, entry->name, rect, 24, 1.0f, false, WHITE);
            //DrawText(entry->name, rect.x, rect.y, 20, WHITE);
        }
    }
}

void alModelUiRenderRtt(AlModelUi *self) {
    alRttRenderTexture(self->view);
}