#pragma once

#include"../rtt.h"
#include"../rldata.h"
#include"../object.h"

class AlModelPreview {
private:

	AlRtt displayTexture;
    AlObject object;

	// For now let's use the same camera always
	Camera camera;

	// If this is true, then display texture has already been rendered once
	// can proceed to render the rtt directly.
	bool displayTextureRenderedOnce = false;
public:
	AlModelPreview(std::shared_ptr<RlModel> model, std::shared_ptr<AlPbrShader> shader);

	void renderToTexture();

	void renderTexture();

	void setRect(Rectangle rect);
};