//
// Created by alether on 10/12/23.
//

#ifndef HELLO_C_UNICODE_FONT_H
#define HELLO_C_UNICODE_FONT_H

#include<raylib.h>
#include"types.h"
#include"array.h"
#include"hashmap.h"

typedef struct AlUnicodeFont {
    AlArray fonts; // AlArray<Font*>, owning
    AlHashMap codepointToFontMap; // AlHashMap<char*, Font**>
} AlUnicodeFont;

typedef struct AlUnicodeFontRange {
    i32 start;
    i32 end;
} AlUnicodeFontRange;

bool alUnicodeFontInit(AlUnicodeFont *self,
                       const char *filePath,
                       i32 fontSize,
                       AlArray ranges /*AlArray<AlUnicodeFontRange> */);

Rectangle alUnicodeFontMeasure(AlUnicodeFont *self, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap);

// return the box
Rectangle alUnicodeFontRenderBoxed(AlUnicodeFont *self, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

Rectangle alUnicodeFontRenderSelectable(AlUnicodeFont *self, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap,
                                   Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint, bool shouldRender);

void alUnicodeFontDeinit(AlUnicodeFont *self);

#endif //HELLO_C_UNICODE_FONT_H
