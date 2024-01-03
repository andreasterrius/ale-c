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
#include "../object.h"
#include "model_preview.h"

// Contains some metadata for the UI
struct AlModelUi_Entry {
    std::string modelId;
    std::string modelPath;
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
    /// The UI will constantly poll watchDirPath when timeToWatchElapsedMs > timeToWatchMs
    std::string watchDirPath;
    float timeToWatchMs = 2.0f;
    float timeToWatchElapsedMs = 0.0f;

    AlRtt view;
    std::shared_ptr<AlUnicodeFont> unicodeFont;
    bool shouldRelayout = false;

public:

    AlModelUi(std::shared_ptr<AlUnicodeFont> font, Rectangle normalizedDest, std::string watchDirPath);

    void tick(float dt);

    void renderToTexture();

    void renderTexture();

    std::vector<AlObject> getSpawnedObjects();
};


#endif //HELLO_C_MODEL_PANEL_H
