//
// Created by alether on 10/3/23.
//

#ifndef HELLO_C_VIEWPORT_H
#define HELLO_C_VIEWPORT_H

#include<raymath.h>
#include<raylib.h>

typedef struct AlViewport {
    // Weak reference to original window size
    // For now we always assume parent origin (0,0)
    Vector2 *parentSize;

    Vector2 origin;
    Vector2 size;

} AlViewport;

Vector2 alViewportGetMousePosition(AlViewport self) {
    Vector2 mousePos = GetMousePosition();
    Vector2 resultPos = (Vector2){
            .x = self.parentSize->x / self.size.x * (self.origin.x + mousePos.x),
            .y = self.parentSize->y / self.size.y * (self.origin.y + mousePos.y)
    };
    return resultPos;
}

#endif //HELLO_C_VIEWPORT_H
