//
// Created by Alether on 1/23/2024.
//

#ifndef HELLO_C_DEFAULT_STUFF_H
#define HELLO_C_DEFAULT_STUFF_H

#include"pbr_shader.h"
#include"unicode_font.h"
#include<memory>
#include<unordered_map>
#include<string>

using namespace std;

struct DefaultStuff {
    shared_ptr<AlUnicodeFont> unicodeFont;
    shared_ptr<AlPbrShader> pbrShader;

    DefaultStuff(){
        this->unicodeFont = make_shared<AlUnicodeFont>(
                "resources/font/NotoSansCJKjp-VF.ttf",
                24,
                std::vector{AlUnicodeFontRange{.start = 0, .end=255}}
        );
        this->pbrShader = make_shared<AlPbrShader>(
                LoadShader("resources/shaders/pbr.vert", "resources/shaders/pbr.frag"));
    }
};

#endif //HELLO_C_DEFAULT_STUFF_H
