//
// Created by alether on 10/12/23.
//

#include "unicode_font.h"
#include "defer.h"
#include<xxhash.h>


void alUnicodeFontRenderBoxed(AlUnicodeFont *self, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint){
    alUnicodeFontRenderSelectable(self, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

// Copied directly from raylib example
void alUnicodeFontRenderSelectable(AlUnicodeFont *self, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap,
                                   Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint) {
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0.0f;       // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize / (float) self->baseFont.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum {
        MEASURE_STATE = 0, DRAW_STATE = 1
    };
    int state = wordWrap ? MEASURE_STATE : DRAW_STATE;

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++) {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        khiter_t iter = kh_get(fontCache, self->codepointToFontMap, codepoint);

        Font *font;
        int index;
        if(iter == kh_end(self->codepointToFontMap)){
            font = kh_val(self->codepointToFontMap, iter);
            index = GetGlyphIndex(*font, codepoint);
        } else {
            index = 0x3f; // '?'
            font = &self->baseFont;
        }

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n') {
            glyphWidth = (font->glyphs[index].advanceX == 0) ? font->recs[index].width * scaleFactor :
                         font->glyphs[index].advanceX * scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container).
        // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
        // and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE) {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width) {
                endLine = (endLine < 1) ? i : endLine;
                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                state = !state;
            } else if ((i + 1) == length) {
                endLine = i;
                state = !state;
            } else if (codepoint == '\n') state = !state;

            if (state == DRAW_STATE) {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = iter - 1;
                iter = tmp;
            }
        } else {
            if (codepoint == '\n') {
                if (!wordWrap) {
                    textOffsetY += (font->baseSize + font->baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                }
            } else {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width)) {
                    textOffsetY += (font->baseSize + font->baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font->baseSize * scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (iter >= selectStart) && (iter < (selectStart + selectLength))) {
                    DrawRectangleRec((Rectangle) {rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth,
                                                  (float) font->baseSize * scaleFactor}, selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t')) {
                    DrawTextCodepoint(*font, codepoint, (Vector2) {rec.x + textOffsetX, rec.y + textOffsetY}, fontSize,
                                      isGlyphSelected ? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine)) {
                textOffsetY += (font->baseSize + font->baseSize / 2) * scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - iter;
                iter = lastk;

                state = !state;
            }
        }

        textOffsetX += glyphWidth;
    }
}

bool alUnicodeFontInit(AlUnicodeFont *self,
                       const char *filePath,
                       i32 fontSize,
                       AlTextRendererFontRangeVec *ranges) {
    // Loading file to memory
    u32 fileSize = 0;
    unsigned char *fileData = LoadFileData(filePath, &fileSize);
    char *fileExt = GetFileExtension(filePath);

    i32 maxTextureSize = 8192;
    i32 glyphCountPerTexture = (8192 / fontSize) + 1;

    for (int i = 0; i < kv_size(*ranges); ++i) {
        AlUnicodeFontRange range = kv_A(*ranges, i);
        kvec_t(int) codePointToLoad;
        kv_init(codePointToLoad);
        defer{ kv_destroy(codePointToLoad); };

        int count = 0;
        Font *font = (Font *) malloc(sizeof(Font));

        for (i32 j = range.start; j < range.end; j += 1) {
            i32 ret;
            i32 k = kh_put(fontCache, self->codepointToFontMap, j, &ret);
            kh_value(self->codepointToFontMap, k) = font;

            // load the glyphs
            // - when its too much for one font atlas or
            // - when this is last codepoint and haven't created the font atlas yet
            if (count > glyphCountPerTexture || (j == range.end - 1 && count != 0)) {
                Font tempFont = LoadFontFromMemory(fileExt, fileData, fileSize, fontSize, codePointToLoad.a, count);
                *font = tempFont;
                kv_push(Font*, self->fonts, font);

                //reset all
                count = 0;
                kv_destroy(codePointToLoad);
                kv_init(codePointToLoad);

                // save this for default fallback
                self->baseFont = tempFont;
            }
        }
    }

    // nothing is loaded
    if(kv_size(self->fonts) == 0) {
        // log error here
        return false;
    }

    return true;
}

void alUnciodeFontDeinit(AlUnicodeFont *self) {
    kh_destroy(fontCache, self->codepointToFontMap);
    for (int i = 0; i < kv_size(self->fonts); ++i) {
        free(kv_A(self->fonts, i));
    }
    kv_destroy(self->fonts);
}
