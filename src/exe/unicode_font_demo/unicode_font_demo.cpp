#include <raylib.h>
#include "../../unicode_font.h"

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello unicode fonts");


    SetTargetFPS(60);

    std::vector<AlUnicodeFontRange> ranges{
        (AlUnicodeFontRange) {.start = 0, .end=255},
        (AlUnicodeFontRange) {.start = 12352, .end=12543},
        (AlUnicodeFontRange) {.start = 19968, .end=40959},
        (AlUnicodeFontRange) {.start = 44032, .end=55215}
    };

    AlUnicodeFont customFont("resources/font/NotoSansCJKjp-VF.ttf", 24, ranges);

    Rectangle panel = (Rectangle) {.x=0, .y=0, .width=200, .height=200};

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawRectangle(panel.x, panel.y, panel.width, panel.height, RED);
        customFont.renderBoxed("test | 밥을 먹다 | ご飯を食べる", panel, 24, 1.0f, false, WHITE);

        ClearBackground(SKYBLUE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}