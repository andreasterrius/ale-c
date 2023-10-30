#include "defer.h"
#include <raylib.h>
#include "unicode_font.h"

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello unicode fonts");
    defer{ CloseWindow(); };

    SetTargetFPS(60);

    AlArray ranges;
    alArrayInit(&ranges, sizeof(AlUnicodeFontRange), 3);
    defer{ alArrayDeinit(&ranges); };
    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 12352, .end=12543}));
    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 19968, .end=40959}));
    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 116, .end=300}));
//    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 100, .end=200}));
//    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 400, .end=500}));
//    alArrayPush(&ranges, &((AlUnicodeFontRange) {.start = 600, .end=800}));


    AlUnicodeFont customFont;
    bool ok = alUnicodeFontInit(&customFont, "resources/font/NotoSansCJKjp-VF.ttf", 24, ranges);
    if (!ok) {
        return -1;
    }
    defer{ alUnicodeFontDeinit(&customFont); };

    Rectangle panel = (Rectangle) {.x=0, .y=0, .width=200, .height=200};

    for(int i = 0; i < customFont.codepointToFontMap.tableSize; ++i) {
        AlKeyValuePair p = customFont.codepointToFontMap.table[i];

        int codepoint = 12372;
        char buffer[12];
        sprintf(buffer, "%d", codepoint);
        if(p.key != NULL && strcmp(buffer, p.key) == 0){
            printf("hit"); //11446
        }
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        defer{ EndDrawing(); };

        DrawRectangle(panel.x, panel.y, panel.width, panel.height, RED);
        alUnicodeFontRenderBoxed(&customFont, "test | 밥을 먹다 | ご飯を食べる", panel, 24, 1.0f, false, WHITE);
//        alUnicodeFontRenderBoxed(&customFont, "ごべる", panel, 24, 1.0f, false, WHITE);

        //DrawTextBoxed(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", panel, fontEastern.baseSize, 1.0f, true, WHITE);
        //DrawTextEx(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", Vector2Zero(), fontEastern.baseSize, 1.0f, BLACK);

        //alTextRendererRender(&customFont, "test | 밥을 먹다 | ご飯を食べる", (Rectangle){.x=0,.y=0,.width=0,.height=0});

        // DrawTexture(texture2D, 0, 0, WHITE);

        ClearBackground(SKYBLUE);
    }

    return 0;
}