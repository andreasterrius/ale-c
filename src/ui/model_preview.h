#pragma once

#include"../rtt.h"
#include"../rldata.h"
#include"../rldata.h"

class AlModelPreview {
private:

	AlRtt displayTexture;
	std::shared_ptr<RlModel> model;

	// For now let's use the same camera always
	Camera camera;

	// If this is true, then display texture has already been rendered once
	// can proceed to render the rtt directly.
	bool displayTextureRenderedOnce = false;
public:
	explicit AlModelPreview(std::shared_ptr<RlModel> model);

	void renderToTexture();

	void renderTexture();

	void setRect(Rectangle rect);
};