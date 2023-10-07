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

int main(){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello render to texture");
    defer{ CloseWindow(); };

    SetTargetFPS(60);



    while (!WindowShouldClose()) {
        BeginDrawing();
        defer { EndDrawing(); };


        ClearBackground(SKYBLUE);
    }
    return 0;
}