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
#include"ui.h"

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello fonts");
    defer{ CloseWindow(); };

    SetTargetFPS(60);

//    // JSON data as a string
//    const char *json_string = "{\"name\": \"John\", \"age\": 30}";
//
//    // Parse JSON data
//    json_object *json_obj = json_tokener_parse(json_string);
//
//    if (json_obj == NULL) {
//        printf("Error parsing JSON\n");
//        return 1;
//    }
//
//    // Access JSON data
//    json_object *name;
//    json_object *age;
//
//    if (json_object_object_get_ex(json_obj, "name", &name) &&
//        json_object_object_get_ex(json_obj, "age", &age)) {
//
//        printf("Name: %s\n", json_object_get_string(name));
//        printf("Age: %d\n", json_object_get_int(age));
//    } else {
//        printf("JSON data structure is incorrect\n");
//    }
//
//    // Free the JSON object
//    json_object_put(json_obj);
    Font fontEastern = LoadFont("resources/font/NotoSansCJKjp-VF.ttf");
    defer{ UnloadFont(fontEastern); };

    Rectangle panel = (Rectangle) {.x=200, .y=200, .width=400, .height=200};

    while (!WindowShouldClose()) {
        BeginDrawing();
        defer{ EndDrawing(); };

        DrawRectangle(panel.x, panel.y, panel.width, panel.height, RED);
        //DrawTextBoxed(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", panel, fontEastern.baseSize, 1.0f, true, WHITE);
        DrawTextEx(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", Vector2Zero(), fontEastern.baseSize, 1.0f, BLACK);

        LoadFontEx()

        ClearBackground(SKYBLUE);
    }
    return 0;
}