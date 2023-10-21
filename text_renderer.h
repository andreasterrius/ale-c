//
// Created by alether on 10/12/23.
//

#ifndef HELLO_C_TEXT_RENDERER_H
#define HELLO_C_TEXT_RENDERER_H

#include<raylib.h>
#include<extern/klib/>

typedef struct AlTextRenderer {
    PangoFontMap *fontMap;
    PangoContext *context;
    PangoLayout *defaultLayout;
    PangoFontDescription *fontDescription;

    // Cache
    GHashTable *textCache;

} AlTextRenderer;

typedef struct AlTextRendererCacheKey {
    char *text;
    int rectWidth;
    int rectHeight;
} AlTextRendererCacheKey;


// Custom hash function using XXHash for your key type
guint alTextRendererCacheKeyHash(gconstpointer key);

gboolean alTextRendererCacheKeyEqual(gconstpointer a, gconstpointer b);

void alTextRendererInit(AlTextRenderer *self);

void alTextRendererDeinit(AlTextRenderer *self);

void alTextRendererRender(AlTextRenderer *self, char *word, Rectangle bound);

#endif //HELLO_C_TEXT_RENDERER_H
