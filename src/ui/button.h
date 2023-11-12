//
// Created by alether on 2023/11/12.
//

#ifndef HELLO_C_BUTTON_H
#define HELLO_C_BUTTON_H

#include <raylib.h>
#include "../../types.h"

typedef struct AlButton {
    Rectangle rect;
    char* text;
    Color bgColor;
    Color pressedBgColor;

    bool isPressedThisFrame;
} AlButton;

void alButtonInit(AlButton *self, char *name);

void alButtonTick(AlButton *self, Vector2 mousePosition);

void alButtonDeinit(AlButton *self);

#endif //HELLO_C_BUTTON_H
