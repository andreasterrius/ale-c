#include "defer.h"
#include <raylib.h>
#include "unicode_font.h"

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello unicode fonts");
    defer{ CloseWindow(); };

    SetTargetFPS(60);

    AlTextRendererFontRangeVec ranges;
    kv_init(ranges);
    defer{ kv_destroy(ranges); };
    kv_push(AlUnicodeFontRange, ranges, ((AlUnicodeFontRange) {.start = 19968, .end=40959}));

    AlUnicodeFont textRenderer;
    bool ok = alUnicodeFontInit(&textRenderer, "resources/font/NotoSansCJKjp-VF.ttf", 24, &ranges);
    if (!ok) {
        return -1;
    }
    defer{ alUnciodeFontDeinit(&textRenderer); };

    Rectangle panel = (Rectangle) {.x=0, .y=0, .width=200, .height=200};

    while (!WindowShouldClose()) {
        BeginDrawing();
        defer{ EndDrawing(); };

        DrawRectangle(panel.x, panel.y, panel.width, panel.height, RED);
        alUnicodeFontRenderSelectable()
        //DrawTextBoxed(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", panel, fontEastern.baseSize, 1.0f, true, WHITE);
        //DrawTextEx(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", Vector2Zero(), fontEastern.baseSize, 1.0f, BLACK);

        //alTextRendererRender(&textRenderer, "test | 밥을 먹다 | ご飯を食べる", (Rectangle){.x=0,.y=0,.width=0,.height=0});

        // DrawTexture(texture2D, 0, 0, WHITE);

        ClearBackground(SKYBLUE);
    }

    alUnciodeFontDeinit(&textRenderer);
    CloseWindow();

    return 0;
}