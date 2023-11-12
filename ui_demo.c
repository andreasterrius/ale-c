//
// Created by alether on 10/7/23.
//
#include<stdio.h>
#include<raylib.h>
#include<raymath.h>
#include <stdio.h>
#include"rtt.h"
#include"arc_camera.h"
#include"defer.h"
#include<json.h>
#include"array.h"

typedef struct AlModelListPanel {
    AlRtt rtt;
    Color backgroundColor;

} AlModelListPanel;

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    int width = 800;
    int height = 600;
    InitWindow(width, height, "Hello UI layouts");
    defer{ CloseWindow(); };

    SetTargetFPS(60);

    AlRtt editorRtt;
    alRttInit(&editorRtt, (Rectangle){
        .x = 0, .y = 0, .height=height, .width=width * 0.7f
    });

    AlModelListPanel modelListPanel;
    alRttInit(&modelListPanel.rtt, (Rectangle){
            .x = editorRtt.actualDest.width, .y = 0, .height=height, .width=width * 0.3f
    });
    modelListPanel.backgroundColor = RED;

    while (!WindowShouldClose()) {

        // Render editor
        {
            alRttBeginRenderToTexture(editorRtt);
            defer{ alRttEndRenderToTexture(editorRtt); };

            // Render scene here
            DrawRectangle(0, 0, 800, 600, GREEN);
        }

        // Render UI
        {
            alRttBeginRenderToTexture(modelListPanel.rtt);
            defer{ alRttEndRenderToTexture(modelListPanel.rtt); };

            // Render UI here
            DrawRectangle(0, 0, 800, 600, modelListPanel.backgroundColor);
        }

        // Render RTT's to screen
        BeginDrawing();
        defer{ EndDrawing(); };
        alRttRenderTexture(modelListPanel.rtt);
        alRttRenderTexture(editorRtt);

        ClearBackground(BLACK);
    }
    return 0;
}