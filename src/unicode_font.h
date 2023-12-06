//
// Created by alether on 10/12/23.
//

#ifndef HELLO_C_UNICODE_FONT_H
#define HELLO_C_UNICODE_FONT_H

#include<raylib.h>
#include"rldata.h"
#include"types.h"
#include<vector>
#include<unordered_map>
#include<string>

class AlUnicodeFontRange {
public:
    i32 start;
    i32 end;
};

class AlUnicodeFont {

public:
    // disable copy ctor
    AlUnicodeFont (const AlUnicodeFont&) = delete;
    AlUnicodeFont& operator= (const AlUnicodeFont&) = delete;

    std::vector<RlFont> fonts;
    std::unordered_map<std::string, i32> codepointToFontMap;

    AlUnicodeFont(const char *filePath,
                  i32 fontSize,
                  std::vector<AlUnicodeFontRange> ranges);

    Rectangle measure(const char *text, Rectangle rec, float fontSize, float spacing,
                      bool wordWrap);

    Rectangle renderBoxed(const char *text, Rectangle rec, float fontSize, float spacing,
                          bool wordWrap, Color tint);

    Rectangle renderSelectable(const char *text, Rectangle rec, float fontSize, float spacing,
                               bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint,
                               Color selectBackTint, bool shouldRender);

};

#endif //HELLO_C_UNICODE_FONT_H
