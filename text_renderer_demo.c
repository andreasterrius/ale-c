#include "defer.h"
#include <raylib.h>
#include "text_renderer.h"

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello fonts");
    //defer{ CloseWindow(); };

    SetTargetFPS(60);

    AlTextRenderer textRenderer;
    alTextRendererInit(&textRenderer);
    //defer{ alTextRendererDeinit(&textRenderer); };

    Rectangle panel = (Rectangle) {.x=0, .y=0, .width=200, .height=200};

    while (!WindowShouldClose()) {
        BeginDrawing();
        defer{ EndDrawing(); };

        DrawRectangle(panel.x, panel.y, panel.width, panel.height, RED);
        //DrawTextBoxed(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", panel, fontEastern.baseSize, 1.0f, true, WHITE);
        //DrawTextEx(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", Vector2Zero(), fontEastern.baseSize, 1.0f, BLACK);

        alTextRendererRender(&textRenderer, "test | 밥을 먹다 | ご飯を食べる", (Rectangle){.x=0,.y=0,.width=0,.height=0});

        // DrawTexture(texture2D, 0, 0, WHITE);

        ClearBackground(SKYBLUE);
    }

    alTextRendererDeinit(&textRenderer);
    CloseWindow();

    return 0;
}