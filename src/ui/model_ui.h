//
// Created by Alether on 11/4/2023.
//

#ifndef HELLO_C_MODEL_PANEL_H
#define HELLO_C_MODEL_PANEL_H

#include <string>
#include <memory>
#include <optional>
#include <unordered_set>
#include"../rldata.h"
#include"../unicode_font.h"
#include"../rtt.h"
#include "button.h"
#include "model_preview.h"

// Contains some metadata for the UI
struct AlModelUi_Entry {
    std::string modelPath; // used for uniqueness check (won't be loaded twice)
    bool isInternal;
    std::shared_ptr<RlModel> model;

    // Will be created after init
    std::optional<AlButton> button;
    std::optional<AlModelPreview> modelPreview;
};

class AlModelUi {
public:
    std::vector<AlModelUi_Entry> modelEntries;

    // ID_xxx or PATH_xxx
    std::unordered_set<std::string> loadedModelIdOrPath;

private:

    // handles scrolling
    float scrollPositionY = 0.0f;

    float minScrollPositionY = 0.0f;
    float maxScrollPositionY = 0.0f; // will be known after first layouting
    float scrollSensitivity = 10.0f;

    /// The UI will constantly poll watchDirPath when timeToWatchElapsedMs > timeToWatchMs
    std::string watchDirPath;
    float timeToWatchMs = 2.0f;
    float timeToWatchElapsedMs = 0.0f;

    AlRtt view;
    std::shared_ptr<AlUnicodeFont> unicodeFont;
    bool shouldRelayout = false;

    std::shared_ptr<AlPbrShader> pbrShader;

public:
    AlModelUi(std::shared_ptr<AlUnicodeFont> font, Rectangle normalizedDest, std::string watchDirPath,
              std::shared_ptr<AlPbrShader> pbrShader);

    void tick(float dt);

    void handleInput();

    void renderToTexture();

    void renderTexture();

    std::vector<AlObject> getSpawnedObjects();
};


#endif //HELLO_C_MODEL_PANEL_H
