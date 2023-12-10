//
// Created by Alether on 11/4/2023.
//

#ifndef HELLO_C_MODEL_PANEL_H
#define HELLO_C_MODEL_PANEL_H

#include <string>
#include <memory>
#include"../rldata.h"
#include"../unicode_font.h"
#include"../rtt.h"
#include "button.h"
#include <optional>
#include "../object.h"

// Contains some metadata for the UI
struct AlModelUi_Entry {
    std::string modelId;
    std::string modelPath;
    std::shared_ptr<RlModel> model;

    // Will be created after init
    std::optional<AlButton> button;
};

class AlModelUi {
public:
    std::vector<AlModelUi_Entry> modelEntries;

private:
    /// The UI will constantly poll watchDirPath when timeToWatchElapsedMs > timeToWatchMs
    std::string watchDirPath;
    float timeToWatchMs = 10.0f;
    float timeToWatchElapsedMs;

    AlRtt view;
    std::shared_ptr<AlUnicodeFont> unicodeFont;
    bool shouldRelayout;

public:

    AlModelUi(std::shared_ptr<AlUnicodeFont> font, Rectangle normalizedDest, std::string watchDirPath);

    void tick(float dt);

    void render();

    void renderRtt();

    std::vector<AlObject> getSpawnedObjects();
};


#endif //HELLO_C_MODEL_PANEL_H
