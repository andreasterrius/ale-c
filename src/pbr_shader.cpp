#include "pbr_shader.h"
#include "pbr_shader.h"
#include "pbr_shader.h"
#include "pbr_shader.h"
#include "pbr_shader.h"
#include <string>

AlPbrShader::AlPbrShader(RlShader rlShader) : rlShader(std::move(rlShader))
{
	this->shaderLoc.resize(AL_PBR_SHADER_LOCATION_MAX);
	this->shaderLoc[AL_ALBEDO_VEC3] = GetShaderLocation(this->rlShader.d, "albedoValue");
	this->shaderLoc[AL_METALLIC_FLOAT] = GetShaderLocation(this->rlShader.d, "metallicValue");
	this->shaderLoc[AL_ROUGHNESS_FLOAT] = GetShaderLocation(this->rlShader.d, "roughnessValue");
	this->shaderLoc[AL_AO_FLOAT] = GetShaderLocation(this->rlShader.d, "aoValue");
	this->shaderLoc[AL_CAM_POS] = GetShaderLocation(this->rlShader.d, "camPos");

	this->shaderLoc[AL_ALBEDO_USE_MAP_BOOL] = GetShaderLocation(this->rlShader.d, "useAlbedoMap");
	this->shaderLoc[AL_METALLIC_USE_MAP_BOOL] = GetShaderLocation(this->rlShader.d, "useMetallicMap");
	this->shaderLoc[AL_ROUGHNESS_USE_MAP_BOOL] = GetShaderLocation(this->rlShader.d, "useRoughnessMap");
	this->shaderLoc[AL_AO_USE_MAP_BOOL] = GetShaderLocation(this->rlShader.d, "useAoMap");
	this->shaderLoc[AL_NORMAL_USE_MAP_BOOL] = GetShaderLocation(this->rlShader.d, "useNormalMap");

	this->rlShader.d.locs[SHADER_LOC_MAP_ALBEDO] = GetShaderLocation(this->rlShader.d, "albedoMap");
	this->rlShader.d.locs[SHADER_LOC_MAP_METALNESS] = GetShaderLocation(this->rlShader.d, "metallicMap");
	this->rlShader.d.locs[SHADER_LOC_MAP_ROUGHNESS] = GetShaderLocation(this->rlShader.d, "roughnessMap");
	this->rlShader.d.locs[SHADER_LOC_MAP_OCCLUSION] = GetShaderLocation(this->rlShader.d, "aoMap");
	this->rlShader.d.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(this->rlShader.d, "normalMap");
}

void AlPbrShader::passScene(Camera camera, std::vector<AlLight>* lights) {
	SetShaderValue(this->rlShader.d, this->shaderLoc[AL_CAM_POS], &camera.position, SHADER_UNIFORM_VEC3);

	for (unsigned int i = 0; i < lights->size(); ++i)
	{
		if (i >= this->lightShaderLoc.size()) {
			this->lightShaderLoc.push_back(AlPbrShader_LightLocation{
				.position = GetShaderLocation(this->rlShader.d, ("lightPositions[" + std::to_string(i) + "]").c_str()),
				.color = GetShaderLocation(this->rlShader.d, ("lightColors[" + std::to_string(i) + "]").c_str()),
			});
		}
		SetShaderValue(this->rlShader.d, this->lightShaderLoc[i].position, &lights->at(i).position, SHADER_UNIFORM_VEC3);
		SetShaderValue(this->rlShader.d, this->lightShaderLoc[i].color, &lights->at(i).colors, SHADER_UNIFORM_VEC3);
	}
}

void AlPbrShader::passVariables(AlPbrMaterial *pbrMaterial) {
	SetShaderValue(this->rlShader.d, this->shaderLoc[AL_ALBEDO_VEC3], &pbrMaterial->albedo, SHADER_UNIFORM_VEC3);
	SetShaderValue(this->rlShader.d, this->shaderLoc[AL_METALLIC_FLOAT], &pbrMaterial->metallic, SHADER_UNIFORM_FLOAT);
	SetShaderValue(this->rlShader.d, this->shaderLoc[AL_ROUGHNESS_FLOAT], &pbrMaterial->roughness, SHADER_UNIFORM_FLOAT);
	SetShaderValue(this->rlShader.d, this->shaderLoc[AL_AO_FLOAT], &pbrMaterial->AO, SHADER_UNIFORM_FLOAT);
	
	int t = 1, f = 0;
	SetShaderValue(this->rlShader.d, this->shaderLoc[AL_ALBEDO_USE_MAP_BOOL], pbrMaterial->albedoMap.has_value() == true ? &t : &f, SHADER_UNIFORM_INT);

}

AlPbrMaterial::AlPbrMaterial(std::shared_ptr<AlPbrShader> pbrShader) 
	: pbrShader(std::move(pbrShader))
{
}

void AlPbrMaterial::passVariables()
{
	this->pbrShader->passVariables(this);
}
