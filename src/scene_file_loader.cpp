
#include "scene_file_loader.h"
#include <fstream>

AlSceneFileLoader::AlSceneFileLoader(std::string path) : path(path) {}

bool AlSceneFileLoader::save(std::vector<AlObject> &objects) {
    std::cout << "Saving to " << this->path << "..." << std::endl;

    AlSceneFileLoader_SceneFile sceneFile;
    for (int i = 0; i < objects.size(); ++i) {
        sceneFile.objects.emplace_back(AlSceneFileLoader_ObjectFile{
                .modelPath = objects[i].modelPath,
                .isInternal = objects[i].isInternal,
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
        // if it's an internal model, no need to laod anything, just reference it directly
        if (fileObj->isInternal) {
            if (internalModels.find(fileObj->modelPath) != internalModels.end()) {
                auto model = internalModels[fileObj->modelPath];
                loadedScene.objects.emplace_back(fileObj->transform, model);
                loadedScene.objects.back().modelPath = fileObj->modelPath;
                loadedScene.objects.back().isInternal = true;
            }
            else {
                TraceLog(LOG_WARNING, ("An internal model was skipped in loading because invalid modelPath: " + fileObj->modelPath).c_str());
            }
        }
       
        // this is an external model for sure
        if (!fileObj->isInternal) {
            if (loadedModels.find(fileObj->modelPath) == loadedModels.end()) {
                auto model = std::make_shared<RlModel>(LoadModel(fileObj->modelPath.c_str()));
                loadedScene.objects.emplace_back(fileObj->transform, model);
                loadedScene.objects.back().modelPath = fileObj->modelPath;
                loadedScene.objects.back().isInternal = false;

                loadedModels[fileObj->modelPath] = model;
            }
            else {
                TraceLog(LOG_WARNING, ("Has been loaded before, skip loading modelPath: " + fileObj->modelPath).c_str());
                auto model = loadedModels[fileObj->modelPath];
                loadedScene.objects.emplace_back(fileObj->transform, model);
                loadedScene.objects.back().modelPath = fileObj->modelPath;
                loadedScene.objects.back().isInternal = false;
            }
        } 
    }
    return std::make_optional(std::move(loadedScene));
}
