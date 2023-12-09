//
// Created by Alether on 12/9/2023.
//

#ifndef HELLO_C_RLJSON_H
#define HELLO_C_RLJSON_H

#include "rldata.h"
#include <nlohmann/json.hpp>

// raylib Vector3
template<>
struct nlohmann::adl_serializer<Vector3> {
    static void to_json(json &j, const Vector3 &value) {
        j["x"] = value.x;
        j["y"] = value.x;
        j["z"] = value.x;
    }

    static void from_json(const json &j, Vector3 &value) {
        value.x = j.at("x").get_to(value.x);
        value.y = j.at("y").get_to(value.y);
        value.z = j.at("z").get_to(value.z);
    }
};

// raylib Quaternion
template<>
struct nlohmann::adl_serializer<Quaternion> {
    static void to_json(json &j, const Quaternion &value) {
        j["x"] = value.x;
        j["y"] = value.x;
        j["z"] = value.x;
        j["w"] = value.w;
    }

    static void from_json(const json &j, Quaternion &value) {
        value.x = j.at("x").get_to(value.x);
        value.y = j.at("y").get_to(value.y);
        value.z = j.at("z").get_to(value.z);
        value.w = j.at("w").get_to(value.w);
    }
};

//raylib Transform
template<>
struct nlohmann::adl_serializer<Transform> {
    static void to_json(json &j, const Transform &value) {
        j = json{
                {"translation", value.translation},
                {"rotation",    value.rotation},
                {"scale",       value.scale}
        };
    }

    static void from_json(const json &j, Transform &value) {
        j.at("translation").get_to(value.translation);
        j.at("rotation").get_to(value.rotation);
        j.at("scale").get_to(value.scale);
    }
};


#endif //HELLO_C_RLJSON_H
