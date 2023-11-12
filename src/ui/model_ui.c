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

void alModelUiTick(AlModelUi *self) {

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
    alRttBeginRenderToTexture(self->view);
    {
        DrawRectangle(0, 0, self->view.actualDest.width, self->view.actualDest.height, BLACK);

        for(int i = 0; i < alArraySize(self->modelList); ++i) {
            AlModelUiEntry *entry = (AlModelUiEntry*) alArrayGet(self->modelList, i);
            Rectangle measuredRect = alUnicodeFontMeasure(self->unicodeFont, entry->name, rect,24, 1.0f, false);

            DrawRectangle(measuredRect.x, measuredRect.y, measuredRect.width, measuredRect.height, BLUE);
            alUnicodeFontRenderBoxed(self->unicodeFont, entry->name, rect,24, 1.0f, false, WHITE);
            rect.y = measuredRect.y + measuredRect.height;
        }
    }
    alRttEndRenderToTexture(self->view);
}

void alModelUiRenderRtt(AlModelUi *self) {
    alRttRenderTexture(self->view);
}