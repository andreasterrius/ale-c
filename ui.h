//
// Created by alether on 10/7/23.
//
#pragma once

#include <raylib.h>
#include <raymath.h>


typedef struct AlUiPanel {
    Vector2 position;
} AlUiPanel;

typedef struct AlUiItem {

} AlUiItem ;

// Copied directly from Raylib Example
// Draw text using font inside rectangle limits
void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

// Copied directly from Raylib Example
// Draw text using font inside rectangle limits with support for text selection
void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);