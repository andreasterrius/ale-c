//
// Created by alether on 2023/11/12.
//

#include "button.h"

#include <utility>

AlButton::AlButton(std::string label, std::shared_ptr<AlUnicodeFont> font, Color color) :
        label(std::move(label)), font(std::move(font)),
        currentColor(color), normalColor(color), hoverColor(color), clickedColor(color) {

}

bool AlButton::isClicked(Vector2 mousePosition, bool isMouseClicked) {
    if (this->rect.x < mousePosition.x && mousePosition.x > this->rect.x + this->rect.width &&
        this->rect.y > mousePosition.y && mousePosition.y < this->rect.y + this->rect.height) {
        return true;
    }
    return false;
}

void AlButton::render() {
    Rectangle measuredRect = this->font->measure(label.c_str(), rect, 24, 1.0f, false);

    DrawRectangle(measuredRect.x, measuredRect.y, measuredRect.width, measuredRect.height, BLUE);
    this->font->renderBoxed(label.c_str(), rect, 24, 1.0f, false, WHITE);
    rect.y = measuredRect.y + measuredRect.height;

}

void AlButton::setRect(Rectangle rect) {
    this->rect = rect;
}
