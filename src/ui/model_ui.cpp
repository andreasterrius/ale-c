//
// Created by Alether on 11/5/2023.
//
#include "model_ui.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

AlModelUi::AlModelUi(std::shared_ptr<AlUnicodeFont> unicodeFont, Rectangle normalizedDest, std::string watchDirPath) :
        unicodeFont(std::move(unicodeFont)), view(normalizedDest), watchDirPath(watchDirPath) {
}

void AlModelUi::tick(float dt) {
    // check whether we need to re-check the filesystem
    this->timeToWatchElapsedMs += dt;
    if(this->timeToWatchElapsedMs > timeToWatchMs) {
        try {
            for (const auto &entry: fs::directory_iterator(this->watchDirPath)) {
                std::cout << entry.path() << std::endl;
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error accessing directory: " << e.what() << std::endl;
        }
    }

    //if button has not been created yet, let's create it
    for(int i = 0; i < this->loadedModelEntries.size(); i++){
        AlModelUi_Entry *entry = &this->loadedModelEntries[i];
        if(!entry->button){
            entry->button = AlButton("", this->unicodeFont, GREEN);
        }

        // TODO: if button has been created, let's check it's collission box!
    }
}

void AlModelUi::render() {
    bool shouldRelayout = this->view.tryRecalculateRect();

    // Draw Texts
    this->view.beginRenderToTexture();
    {
        DrawRectangle(0, 0, this->view.actualDest.width, this->view.actualDest.height, BLACK);

        for (int i = 0; i < this->loadedModelEntries.size(); ++i) {
            AlModelUi_Entry *entry = &this->loadedModelEntries[i];

            if(entry->button) {
                if(shouldRelayout || (entry->button->rect.height == 0 && entry->button->rect.width == 0)) {
                    //TODO: calculate the button (position, size here)
                }
                entry->button->render();
            }
        }
    }
    this->view.endRenderToTexture();
}

void AlModelUi::renderRtt() {
    this->view.renderTexture();
}