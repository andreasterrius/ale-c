

#ifndef HELLO_C_SCENE_FILE_LOADER_H
#define HELLO_C_SCENE_FILE_LOADER_H

#include<nlohmann/json.hpp>
#include"rlmath.h"
#include"rljson.h"
#include"object.h"
#include<string>
#include<unordered_map>
#include<optional>

struct AlSceneFileLoader_ObjectFile {
    std::string modelId; // for internal
    std::string modelPath; // for external
    Transform transform;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AlSceneFileLoader_ObjectFile, modelId, modelPath, transform)
};

struct AlSceneFileLoader_SceneFile {
    std::vector<AlSceneFileLoader_ObjectFile> objects;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AlSceneFileLoader_SceneFile, objects)
};

struct AlSceneFileLoader_LoadedScene {
    std::unordered_map<std::string, std::shared_ptr<RlModel>> models;
    std::vector<AlObject> objects;
};

class AlSceneFileLoader {
private:
    std::string path;
public:
    AlSceneFileLoader(std::string path);

    bool save(std::vector<AlObject> &objects);

    std::optional<AlSceneFileLoader_LoadedScene> load();
};


#endif //HELLO_C_SCENE_FILE_LOADER_H
