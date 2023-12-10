//
// Created by Alether on 11/5/2023.
//
#include "model_ui.h"
#include "../rlmath.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

AlModelUi::AlModelUi(std::shared_ptr<AlUnicodeFont> unicodeFont, Rectangle normalizedDest, std::string watchDirPath) :
        unicodeFont(std::move(unicodeFont)), view(normalizedDest), watchDirPath(watchDirPath) {
}

void AlModelUi::tick(float dt) {
    // check whether we need to re-check the filesystem
    this->timeToWatchElapsedMs += dt;
    if (this->timeToWatchElapsedMs > timeToWatchMs) {
        try {
            for (const auto &entry: fs::directory_iterator(this->watchDirPath)) {
                std::cout << entry.path() << std::endl;
            }
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Error accessing directory: " << e.what() << std::endl;
        }
    }

    //if button has not been created yet, let's create it
    for (int i = 0; i < this->modelEntries.size(); i++) {
        AlModelUi_Entry *entry = &this->modelEntries[i];
        if (!entry->button) {
            entry->button = AlButton(entry->modelId, this->unicodeFont, LIGHTGRAY, BLACK);
            entry->button->hoverColor = PURPLE;
            entry->button->clickedColor = DARKPURPLE;
            this->shouldRelayout = true;
        }
        entry->button->tick(this->view.getLocalMousePos(), IsMouseButtonDown(MOUSE_BUTTON_LEFT));
    }

    // do relayouting here
    if (this->shouldRelayout) {
        float currY = 0;
        for (int i = 0; i < this->modelEntries.size(); i++) {
            AlModelUi_Entry *entry = &this->modelEntries[i];
            Rectangle labelRect = entry->button->measureLabelRect();
            entry->button->rect = Rectangle{0, currY, this->view.actualDest.width, labelRect.height};
            currY += labelRect.height;
        }
        this->shouldRelayout = false;
    }
}

void AlModelUi::render() {
    this->shouldRelayout = this->view.tryRecalculateRect();

    // Draw Buttons
    this->view.beginRenderToTexture();
    {
        DrawRectangle(0, 0, this->view.actualDest.width, this->view.actualDest.height, BLACK);

        for (int i = 0; i < this->modelEntries.size(); ++i) {
            AlModelUi_Entry *entry = &this->modelEntries[i];
            if (entry->button) {
                entry->button->render();
            }
        }
    }
    this->view.endRenderToTexture();
}

void AlModelUi::renderRtt() {
    this->view.renderTexture();
}

std::vector<AlObject> AlModelUi::getSpawnedObjects() {
    std::vector<AlObject> objects;
    for (int i = 0; i < this->modelEntries.size(); ++i) {
        AlModelUi_Entry *entry = &this->modelEntries[i];
        if (entry->button && entry->button->getHasJustBeenPressed()) {
            objects.emplace_back(TransformOrigin(), entry->model);
            objects.back().modelId = entry->modelId;
            objects.back().modelPath = entry->modelPath;
        }
    }
    return objects;
}
