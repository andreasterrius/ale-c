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
#include<pango/pango.h>
#include<pango/pangocairo.h>
#include<cairo.h>

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello fonts");
    defer{ CloseWindow(); };

    SetTargetFPS(60);

    PangoContext *context;
    context = pango_font_map_create_context(pango_cairo_font_map_get_default());

    PangoLayout *layout;
    layout = pango_layout_new(context);

    //PangoLayout *layout = pango_cairo_create_layout(context);
    //PangoContext *context = pango_layout_get_context(layout);
    pango_context_set_base_dir(context, PANGO_DIRECTION_LTR); // Set text direction as needed
    int wrapWidth = 200; // Adjust this width as needed
    pango_layout_set_width(layout, wrapWidth * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
    pango_layout_set_justify(layout, true);



    // Set the text and fonts
    pango_layout_set_text(layout, "test | 밥을 먹다 | ご飯を食べる", -1);
    PangoFontDescription *fontDesc = pango_font_description_new();
    pango_font_description_set_family(fontDesc, "Arial");
    pango_font_description_set_size(fontDesc, 24 * PANGO_SCALE); // Font size
    pango_layout_set_font_description(layout, fontDesc);
    pango_font_description_free(fontDesc);

    int textWidth, textHeight;
    pango_layout_get_pixel_size(layout, &textWidth, &textHeight);

    printf("PEW: %d %d", textWidth, textHeight);

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, textWidth, textHeight);
    cairo_t *cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 0, 0, 0);
    pango_cairo_show_layout(cr, layout);

    cairo_image_surface_get_data(surface);
    Image image = { 0 };
    image.data = cairo_image_surface_get_data(surface);
    image.width = textWidth;
    image.height = textHeight;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // Adjust the format as needed

    Texture2D texture2D = LoadTextureFromImage(image);
    cairo_surface_destroy(surface);
    cairo_destroy(cr);

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

    Rectangle panel = (Rectangle) {.x=0, .y=0, .width=textWidth, .height=textHeight};

    while (!WindowShouldClose()) {
        BeginDrawing();
        defer{ EndDrawing(); };

        DrawRectangle(panel.x, panel.y, panel.width, panel.height, RED);
        //DrawTextBoxed(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", panel, fontEastern.baseSize, 1.0f, true, WHITE);
        //DrawTextEx(fontEastern, "test | 밥을 먹다 | ご飯を食べる\n", Vector2Zero(), fontEastern.baseSize, 1.0f, BLACK);

        DrawTexture(texture2D, 0, 0, WHITE);

        ClearBackground(SKYBLUE);
    }
    return 0;
}