//
// Created by alether on 2023/11/12.
//

#include "button.h"
#include<iostream>
#include <utility>

AlButton::AlButton(std::string label, std::shared_ptr<AlUnicodeFont> font, Color color, Color labelColor) :
        label(std::move(label)), font(std::move(font)),
        currentColor(color), normalColor(color), hoverColor(color), clickedColor(color), labelColor(labelColor) {

}

void AlButton::tick(Vector2 mousePosition, bool isMouseDown) {
    this->isHovered = false;
    this->hasJustBeenPressed = false;
    currentColor = normalColor;
    if (this->rect.x < mousePosition.x && mousePosition.x < this->rect.x + this->rect.width &&
        this->rect.y < mousePosition.y && mousePosition.y < this->rect.y + this->rect.height) {
        this->isHovered = true;
        currentColor = hoverColor;

        if(isMouseDown) {
            this->isHeldDown = true;
        }
    }

    if(this->isHeldDown) {
        currentColor = clickedColor;
        if(!isMouseDown) {
            this->hasJustBeenPressed = true;
            this->isHeldDown = false;
        }
    }
}

Rectangle AlButton::measureLabelRect() {
    return this->font->measure(label.c_str(), rect, 24, 1.0f, false);
}

void AlButton::render() {
    DrawRectangle(this->rect.x, this->rect.y, this->rect.width, this->rect.height, this->currentColor);
    this->font->renderBoxed(label.c_str(), this->rect, 24, 1.0f, false, this->labelColor);
}

bool AlButton::getHasJustBeenPressed() {
    return this->hasJustBeenPressed;
}
