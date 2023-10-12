//
// Created by alether on 10/12/23.
//

#include "text_renderer.h"
#include "defer.h"
#include<xxhash.h>

void alTextRendererInit(AlTextRenderer *self) {

    self->fontMap = pango_cairo_font_map_get_default();
    self->context = pango_font_map_create_context(self->fontMap);
    pango_context_set_base_dir(self->context, PANGO_DIRECTION_LTR);

    self->defaultLayout = pango_layout_new(self->context);
    pango_layout_set_wrap(self->defaultLayout, PANGO_WRAP_WORD);

    self->fontDescription = pango_font_description_new();
    pango_font_description_set_family(self->fontDescription, "Arial");
    pango_font_description_set_size(self->fontDescription, 24 * PANGO_SCALE); // Font size
    pango_layout_set_font_description(self->defaultLayout, self->fontDescription);

    self->textCache = g_hash_table_new_full(
        (GHashFunc) alTextRendererCacheKeyHash,
        (GEqualFunc) alTextRendererCacheKeyEqual,
        g_free,
        g_free
    );
}

void alTextRendererDeinit(AlTextRenderer *self) {
    pango_font_description_free(self->fontDescription);
    g_object_unref(self->defaultLayout);
    g_object_unref(self->context);
    g_object_unref(self->fontMap);

    g_hash_table_destroy(self->textCache);
}

void alTextRendererRender(AlTextRenderer *self, char *word, Rectangle bound) {
    pango_layout_set_text(self->defaultLayout, word, -1);

    AlTextRendererCacheKey lookupKey;
    lookupKey.text = word;
    lookupKey.rectHeight = bound.height;
    lookupKey.rectWidth = bound.width;

    gpointer value = g_hash_table_lookup(self->textCache, &lookupKey);
    if(value == NULL) {
        // Create image
        int textWidth, textHeight;
        pango_layout_get_pixel_size(self->defaultLayout, &textWidth, &textHeight);
        cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, textWidth, textHeight);
        defer{ cairo_surface_destroy(surface); };

        cairo_t *cr = cairo_create(surface);
        defer{ cairo_destroy(cr); };

        cairo_set_source_rgb(cr, 0, 0, 0);
        pango_cairo_show_layout(cr, self->defaultLayout);

        Image image = {0};
        image.data = cairo_image_surface_get_data(surface);
        image.width = textWidth;
        image.height = textHeight;
        image.mipmaps = 1;
        image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; // Adjust the format as needed
        Texture2D texture2D = LoadTextureFromImage(image);

        Texture2D *cacheTexture = g_new0(Texture2D, 1);
        cacheTexture->height = texture2D.height;
        cacheTexture->width = texture2D.width;
        cacheTexture->format = texture2D.format;
        cacheTexture->id = texture2D.id;
        cacheTexture->mipmaps = texture2D.mipmaps;

        AlTextRendererCacheKey *key = g_new0(AlTextRendererCacheKey, 1);
        key->text = lookupKey.text;
        key->rectHeight = lookupKey.rectHeight;
        key->rectWidth = lookupKey.rectWidth;

        g_hash_table_insert(self->textCache, key, cacheTexture);
        value = cacheTexture;
    }

    Texture2D *texture = (Texture2D*) value;
    DrawTexture(*texture, bound.x, bound.y, WHITE);
}

gboolean alTextRendererCacheKeyEqual(gconstpointer a, gconstpointer b) {
    const AlTextRendererCacheKey *keyA = (const AlTextRendererCacheKey *) a;
    const AlTextRendererCacheKey *keyB = (const AlTextRendererCacheKey *) b;

    gboolean textEqual = g_str_equal(keyA->text, keyB->text);
    gboolean widthEqual = (keyA->rectWidth == keyB->rectWidth);
    gboolean heightEqual = (keyA->rectHeight == keyB->rectHeight);

    return textEqual && widthEqual && heightEqual;
}

guint alTextRendererCacheKeyHash(gconstpointer key) {
    const AlTextRendererCacheKey *ckey = (const AlTextRendererCacheKey *) key;

    // Create an XXHash state
    XXH64_state_t *state = XXH64_createState();
    defer { XXH64_freeState(state); };
    XXH64_reset(state, 0);

    XXH64_update(state, ckey->text, strlen(ckey->text));
    XXH64_update(state, &ckey->rectWidth, sizeof(int));
    XXH64_update(state, &ckey->rectHeight, sizeof(int));

    // Finalize and return the hash
    return (guint) XXH64_digest(state);
}
