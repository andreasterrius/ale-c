//
// Created by Alether on 11/4/2023.
//

#ifndef HELLO_C_MODEL_PANEL_H
#define HELLO_C_MODEL_PANEL_H

#include"../../array.h"
#include"../../rtt.h"
#include"../../unicode_font.h"
#include"../../defer.h"

typedef struct AlModelUiEntry {
    char* name;
} AlModelUiEntry;

typedef struct AlModelUi {
    char* path;
    AlArray modelList; /* AlArray<AlModelUiEntry> */
    AlUnicodeFont *unicodeFont; /* Non-owning, unicode font has to outlive AlModelUi*/
    AlRtt view;
} AlModelUi;

void alModelUiInit(AlModelUi *self, AlUnicodeFont *unicodeFont, Rectangle normalizedDest);

void alModelUiDeinit(AlModelUi *self);

void alModelUiRender(AlModelUi *self);

void alModelUiRenderRtt(AlModelUi *self);

#endif //HELLO_C_MODEL_PANEL_H
