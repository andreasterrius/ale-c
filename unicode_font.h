//
// Created by alether on 10/12/23.
//

#ifndef HELLO_C_UNICODE_FONT_H
#define HELLO_C_UNICODE_FONT_H

#include<raylib.h>
#include"types.h"

typedef struct AlUnicodeFont {
    khash_t(fontCache) *codepointToFontMap;
    kvec_t(Font*) fonts; //owning pointer for font
    Font* baseFont;
} AlUnicodeFont;

typedef struct AlTextRendererFontRange {
    i32 start;
    i32 end;
} AlUnicodeFontRange;

typedef kvec_t(AlUnicodeFontRange) AlTextRendererFontRangeVec;

bool alUnicodeFontInit(AlUnicodeFont *self,
                       const char *filePath,
                       i32 fontSize,
                       AlTextRendererFontRangeVec *ranges);


void alUnicodeFontRender(AlUnicodeFont *self, const char *text, float fontSize);

void alUnicodeFontRenderBoxed(AlUnicodeFont *self, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

void alUnicodeFontRenderSelectable(AlUnicodeFont *self, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap,
                                   Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);

void alUnicodeFontDeinit(AlUnicodeFont *self);

#endif //HELLO_C_UNICODE_FONT_H
