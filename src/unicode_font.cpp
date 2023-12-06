//
// Created by alether on 10/12/23.
//

#include"unicode_font.h"
#include<vector>

i32 min(i32 a, i32 b) {
    if (a <= b) {
        return a;
    } else {
        return b;
    }
}

i32 max(i32 a, i32 b) {
    if (a >= b) {
        return a;
    } else {
        return b;
    }
}

AlUnicodeFont::AlUnicodeFont(const char *filePath,
                             i32 fontSize,
                             std::vector<AlUnicodeFontRange> ranges) {
    // Loading file to memory
    u32 fileSize = 0;
    unsigned char *fileData = LoadFileData(filePath, &fileSize);
    const char *fileExt = GetFileExtension(filePath);

    i32 glyphCountPerTexture = 1000;
    for (int i = 0; i < ranges.size(); ++i) {
        AlUnicodeFontRange range = ranges[i];

        for (i32 j = range.start; j <= range.end; j += glyphCountPerTexture) {
            i32 start = j;
            i32 end = min(j + glyphCountPerTexture, range.end);

            // for every unicode codepoint, establish connection between glyph->texture
            std::vector<int> codepointToLoad;
            for (int k = start; k <= end; ++k) {
                codepointToLoad.push_back(k);
                this->codepointToFontMap[std::to_string(k)] = this->fonts.size();
            }

            // now we actually load the texture into memory, attach it to Font*
            Font font = LoadFontFromMemory(fileExt, fileData, fileSize, fontSize, &codepointToLoad[0], codepointToLoad.size());
            this->fonts.emplace_back(font);
        }
    }

    UnloadFileData(fileData);
}

Rectangle AlUnicodeFont::measure(const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap) {
    return this->renderSelectable(text, rec, fontSize, spacing, wordWrap, WHITE, 0, 0, WHITE, WHITE, false);
}

Rectangle AlUnicodeFont::renderBoxed(const char *text, Rectangle rec, float fontSize,
                                     float spacing, bool wordWrap, Color tint) {
    return this->renderSelectable(text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE, true);
}

// Copied directly from raylib example
Rectangle AlUnicodeFont::renderSelectable(const char *text, Rectangle rec, float fontSize,
                                          float spacing, bool wordWrap, Color tint, int selectStart, int selectLength,
                                          Color selectTint, Color selectBackTint,
                                          bool shouldRender) {
    if (this->fonts.size() == 0) {
        // no loaded font
        return (Rectangle) {};
    }
    RlFont *baseFont = &this->fonts[0];

    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float maxTextOffsetX = 0.0f;
    float textOffsetY = 0.0f;       // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize / (float) baseFont->d.baseSize;     // Character rectangle scaling factor

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

        i32 fontIndex = this->codepointToFontMap[std::to_string(codepoint)];
        i32 glyphIndex;
        RlFont *font;
        if (fontIndex >= this->fonts.size()) {
            glyphIndex = 0x3f; // '?'
            font = baseFont;
        } else {
            font = &this->fonts[fontIndex];
            glyphIndex = GetGlyphIndex(font->d, codepoint);
        }

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n') {
            glyphWidth = (font->d.glyphs[glyphIndex].advanceX == 0) ? font->d.recs[glyphIndex].width * scaleFactor :
                         font->d.glyphs[glyphIndex].advanceX * scaleFactor;

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
                lastk = k - 1;
                k = tmp;
            }
        } else {
            if (codepoint == '\n') {
                if (!wordWrap) {
                    textOffsetY += (font->d.baseSize + font->d.baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                }
            } else {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width)) {
                    textOffsetY += (font->d.baseSize + font->d.baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font->d.baseSize * scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength))) {
                    if (shouldRender) {
                        DrawRectangleRec((Rectangle) {rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth,
                                                      (float) font->d.baseSize * scaleFactor}, selectBackTint);
                        isGlyphSelected = true;
                    }
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t') && shouldRender) {
                    DrawTextCodepoint(font->d, codepoint, (Vector2) {rec.x + textOffsetX, rec.y + textOffsetY}, fontSize,
                                      isGlyphSelected ? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine)) {
                textOffsetY += (font->d.baseSize + font->d.baseSize / 2) * scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                state = !state;
            }
        }

        textOffsetX += glyphWidth;
        maxTextOffsetX = max(textOffsetX, maxTextOffsetX);
    }

    return (Rectangle) {
            .x = rec.x,
            .y = rec.y,
            .width = maxTextOffsetX,
            .height = textOffsetY + (baseFont->d.baseSize + baseFont->d.baseSize / 2) * scaleFactor,
    };
}
