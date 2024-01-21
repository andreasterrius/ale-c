#pragma once

#include<raylib.h>
#include<vector>
#include<memory>
#include"light.h"
#include"rldata.h"
#include"rlmath.h"
#include"optional"

typedef enum AlPbrShader_Location {
	AL_ALBEDO_VEC3,
	AL_METALLIC_FLOAT,
	AL_ROUGHNESS_FLOAT,
	AL_AO_FLOAT,

	AL_ALBEDO_USE_MAP_BOOL,
	AL_METALLIC_USE_MAP_BOOL,
	AL_ROUGHNESS_USE_MAP_BOOL,
	AL_AO_USE_MAP_BOOL,
	AL_NORMAL_USE_MAP_BOOL,

	AL_CAM_POS,
	AL_PBR_SHADER_LOCATION_MAX,
};

struct AlPbrShader_LightLocation {
	int position;
	int color;
};

class AlPbrShader {
private:
	std::vector<int> shaderLoc;
	std::vector<AlPbrShader_LightLocation> lightShaderLoc;

public:
	RlShader rlShader;

	AlPbrShader(RlShader rlShader);
	
	void passScene(Camera camera, std::vector<AlLight> *lights);

	void passVariables(class AlPbrMaterial *mat);
};

class AlPbrMaterial {
public:
	std::shared_ptr<AlPbrShader> pbrShader;

    // White matte by default
	Vector3 albedo = Vector3{.x=1.0, .y=1.0, .z=1.0};
	float metallic = 0.0f; //metallic, roughness, AO
	float roughness = 1.0f;
	float AO = 1.0f;

	std::optional<std::shared_ptr<RlTexture>> albedoMap;
	std::optional<std::shared_ptr<RlTexture>> metallicMap;
	std::optional<std::shared_ptr<RlTexture>> roughnessMap;
	std::optional<std::shared_ptr<RlTexture>> aoMap;

	// Since the texture is assigned on model level, we need to update it if it's true
	bool needPassToMat;

	AlPbrMaterial(std::shared_ptr<AlPbrShader> pbrShader);

	void passVariables();

};