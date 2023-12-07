//
// Created by alether on 2023/11/12.
//

#ifndef HELLO_C_BUTTON_H
#define HELLO_C_BUTTON_H

#include <raylib.h>
#include <string>
#include "../unicode_font.h"

class AlButton {
public:
    /// This rect should be calculated by outside forces.
    /// The button will never know how to layout itself.
    Rectangle rect;

    std::string label;
    std::shared_ptr<AlUnicodeFont> font;

    Color currentColor;
    Color normalColor;
    Color hoverColor;
    Color clickedColor;

    AlButton(std::string label, std::shared_ptr<AlUnicodeFont> font, Color color);

    // Determine the (position,size) of the button, otherwise it will not be rendered
    void setRect(Rectangle rect);

    bool isClicked(Vector2 mousePosition, bool isMouseClicked);

    void render();
};


#endif //HELLO_C_BUTTON_H
