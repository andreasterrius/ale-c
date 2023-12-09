
#include "scene_file_loader.h"
#include <fstream>

AlSceneFileLoader::AlSceneFileLoader(std::string path) : path(path) {}

bool AlSceneFileLoader::save(std::vector<AlObject> &objects) {
    std::cout << "Saving to " << this->path << "..." << std::endl;

    AlSceneFileLoader_SceneFile sceneFile;
    for (int i = 0; i < objects.size(); ++i) {
        sceneFile.objects.emplace_back(AlSceneFileLoader_ObjectFile{
                .modelId = objects[i].modelId,
                .modelPath = objects[i].modelPath,
                .transform = objects[i].transform
        });
    }

    std::ofstream file(this->path);
    if (file.is_open()) {
        nlohmann::json jsonData = sceneFile;
        file << std::setw(4) << jsonData << std::endl;
        file.close();
        std::cout << "Saving done" << std::endl;
        return true;
    } else {
        std::cout << "Saving failed" << std::endl;
        return false;
    }
}

std::optional<AlSceneFileLoader_LoadedScene> AlSceneFileLoader::load() {
    return std::nullopt;
}
