

#ifndef HELLO_C_SCENE_FILE_LOADER_H
#define HELLO_C_SCENE_FILE_LOADER_H

#include<nlohmann/json.hpp>
#include"rlmath.h"
#include"rljson.h"
#include<string>

struct AlObjectFile {
    std::string modelPath;
    Transform transform;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AlObjectFile, modelPath, transform)
};

struct AlSceneFile {
    std::vector<AlObjectFile> objects;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AlSceneFile, objects)
};

class AlSceneFileLoader {
private:
    std::string path;
public:
    AlSceneFileLoader(std::string path);
};


#endif //HELLO_C_SCENE_FILE_LOADER_H
