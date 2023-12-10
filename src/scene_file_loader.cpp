
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

std::optional<AlSceneFileLoader_LoadedScene>
AlSceneFileLoader::load(std::unordered_map<std::string, std::shared_ptr<RlModel>> &internalModels,
                        std::unordered_map<std::string, std::shared_ptr<RlModel>> &loadedModels) {

    std::cout << "Loading from " << this->path << "..." << std::endl;

    std::ifstream file(this->path);
    if (!file.is_open()) {
        std::cout << "Loading failed" << std::endl;
        return std::nullopt;
    }
    std::cout << "Loading done" << std::endl;
    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    nlohmann::json jsonData = nlohmann::json::parse(fileContents);

    AlSceneFileLoader_SceneFile sceneFile = jsonData;
    //convert scene file to loaded scene
    AlSceneFileLoader_LoadedScene loadedScene;
    for (int i = 0; i < sceneFile.objects.size(); ++i) {
        AlSceneFileLoader_ObjectFile *fileObj = &sceneFile.objects[i];
        // load model first
        if (fileObj->modelId != "" && internalModels.find(fileObj->modelId) != internalModels.end()) {
            auto model = internalModels[fileObj->modelId];
            loadedScene.objects.emplace_back(fileObj->transform, model);
            loadedScene.objects.back().modelId = fileObj->modelId;
        } else if (fileObj->modelPath != "" && loadedModels.find(fileObj->modelPath) != loadedModels.end()) {
            auto model = loadedModels[fileObj->modelPath];
            loadedScene.objects.emplace_back(fileObj->transform, model);
            loadedScene.objects.back().modelPath = fileObj->modelPath;
        } else {
            std::cout << "An object was skipped in loading because invalid modelId: "
                      << fileObj->modelId << "or modelPath:"
                      << fileObj->modelPath << std::endl;
        }
    }
    return std::make_optional(std::move(loadedScene));
}
