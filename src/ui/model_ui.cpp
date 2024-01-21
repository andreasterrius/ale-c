//
// Created by Alether on 11/5/2023.
//
#include "model_ui.h"
#include "../rlmath.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

AlModelUi::AlModelUi(std::shared_ptr<AlUnicodeFont> unicodeFont,
                     Rectangle normRenderRect,
                     std::string watchDirPath,
                     std::shared_ptr<AlPbrShader> pbrShader) :
    unicodeFont(std::move(unicodeFont)), 
    view(normRenderRect),
    watchDirPath(watchDirPath),
    pbrShader(std::move(pbrShader)){
}

void AlModelUi::tick(float dt) {
    // check whether we need to re-check the filesystem
    this->timeToWatchElapsedMs += dt;
    if (this->timeToWatchElapsedMs > timeToWatchMs) {
        try {
            for (const auto &entry: fs::directory_iterator(this->watchDirPath)) {
                if (this->loadedModelIdOrPath.find(entry.path().string()) != this->loadedModelIdOrPath.end()) {
                    continue;
                }
                if (entry.path().extension()  == ".glb" || entry.path().extension() == ".gltf") {
                    this->loadedModelIdOrPath.insert(entry.path().string());
                    Model m = LoadModel(entry.path().string().c_str());
                    this->modelEntries.push_back(AlModelUi_Entry{
                        .modelPath = entry.path().string(),
                        .isInternal = false,
                        .model = std::make_shared<RlModel>(RlModel(m))
                    });
                }
            }
            this->timeToWatchElapsedMs = 0.0f;
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Error accessing directory: " << e.what() << std::endl;
        }
    }
     
    //if button has not been created yet, let's create it
    for (int i = 0; i < this->modelEntries.size(); i++) {
        AlModelUi_Entry *entry = &this->modelEntries[i];
        if (!entry->button) {
            std::string filename = GetFileName(entry->modelPath.c_str());
            entry->button = AlButton(filename, this->unicodeFont, LIGHTGRAY, BLACK);
            entry->button->hoverColor = PURPLE;
            entry->button->clickedColor = DARKPURPLE;
            this->shouldRelayout = true;
        }
        else {
            entry->button->tick(this->view.getLocalMousePos(), IsMouseButtonDown(MOUSE_BUTTON_LEFT));
        }

        if (!entry->modelPreview && entry->button) {
            entry->modelPreview = AlModelPreview(entry->model, this->pbrShader);
            this->shouldRelayout = true;
        }
    }

    // do relayouting here
    if (this->shouldRelayout) {
        float currY = 0;
        for (int i = 0; i < this->modelEntries.size(); i++) {
            AlModelUi_Entry *entry = &this->modelEntries[i];

            // skip if button and modelpreview is not ready
            if (!entry->button.has_value()  || !entry->modelPreview.has_value())
                continue;

            // check size of button label
            Rectangle labelRect = entry->button->measureLabelRect();

            // resize model preview image
            float previewAspect = (float) GetScreenWidth() / GetScreenHeight();
            float previewHeight = 0.1f * this->view.actualDest.height;
            float previewWidth = previewAspect * previewHeight;
            entry->modelPreview->setRect(Rectangle{
                    .x = 0,
                    .y = currY - scrollPositionY,
                    .width = previewWidth,
                    .height = previewHeight,
            });

            // set button size properly
            float buttonHeight = std::max(previewHeight, labelRect.height);
            entry->button->rect = Rectangle{previewWidth, currY - scrollPositionY, this->view.actualDest.width - previewWidth, buttonHeight};

            // prepare next entry
            currY += buttonHeight;
        }
        maxScrollPositionY = currY; 
        this->shouldRelayout = false;
    }
}

void AlModelUi::handleInput() {

    // only handle when mouse is above the render target rect
    // we check this on world position
    Vector2 mousePos = GetMousePosition();
    if (!IsPointInsideRect(this->view.actualDest, mousePos)) {
        return;
    }

    // handle scroll up and down
    float mouseWheelMove = GetMouseWheelMove();
    if (mouseWheelMove != 0.0f) {
        this->shouldRelayout = true;
        scrollPositionY -= mouseWheelMove * scrollSensitivity;

        // validate input and clamp
        if (scrollPositionY < minScrollPositionY) {
            scrollPositionY = 0.0f;
            this->shouldRelayout = false;
        }
        float viewHeight = this->view.actualDest.height;
        if (scrollPositionY + viewHeight > maxScrollPositionY) {
            scrollPositionY = maxScrollPositionY - viewHeight;
            this->shouldRelayout = false;
        }
    }
}

void AlModelUi::renderToTexture() {
    this->shouldRelayout = this->view.tryRecalculateRect();

    // Draw Model Previews
    for (auto &entry : this->modelEntries) {
        if (!entry.modelPreview.has_value()) 
            continue;
        entry.modelPreview->renderToTexture();
    }

    // Draw Buttons
    this->view.beginRenderToTexture();
    {
        DrawRectangle(0, 0, this->view.actualDest.width, this->view.actualDest.height, GREEN);

        for (int i = 0; i < this->modelEntries.size(); ++i) {
            AlModelUi_Entry *entry = &this->modelEntries[i];
            if (entry->button) {
                entry->button->render();
            }
            if (entry->modelPreview) {
                entry->modelPreview->renderTexture();
            }
        }
    }
    this->view.endRenderToTexture();
}

void AlModelUi::renderTexture() {
    this->view.renderTexture();
}

std::vector<AlObject> AlModelUi::getSpawnedObjects() {
    std::vector<AlObject> objects;
    for (int i = 0; i < this->modelEntries.size(); ++i) {
        AlModelUi_Entry *entry = &this->modelEntries[i];
        if (entry->button && entry->button->getHasJustBeenPressed()) {
            objects.emplace_back(TransformOrigin(), entry->model, this->pbrShader);
            objects.back().modelPath = entry->modelPath;
            objects.back().isInternal = entry->isInternal;
        }
    }
    return objects;
}
