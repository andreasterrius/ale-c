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
    Color normalColor;
    Color hoverColor;
    Color clickedColor;

private:
    std::string label;
    std::shared_ptr<AlUnicodeFont> font;
    bool isClicked;
    bool isHovered;
    Color currentColor;

public:
    AlButton(std::string label, std::shared_ptr<AlUnicodeFont> font, Color color);

    bool tick(Vector2 mousePosition, bool isMouseClicked);

    Rectangle measureLabelRect();

    /// render() according to this->rect.
    /// remember that this->rect should be set by outside forces before this is called.
    void render();
};


#endif //HELLO_C_BUTTON_H
