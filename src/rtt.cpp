//
// Created by alether on 10/3/23.
//

#include "rtt.h"


AlRtt::AlRtt(Rectangle normalizedDest) {
	this->normalizedDest = normalizedDest;
	this->actualDest = Rectangle{
			.x = GetScreenWidth() * this->normalizedDest.x,
			.y = GetScreenHeight() * this->normalizedDest.y,
			.width = GetScreenWidth() * this->normalizedDest.width,
			.height = GetScreenHeight() * this->normalizedDest.height,
	};
	this->renderTexture2D = RlRenderTexture2D{ LoadRenderTexture(this->actualDest.width, this->actualDest.height) };
	this->needRecalculateActualDest = false;
}

void AlRtt::beginRenderToTexture() {
	BeginTextureMode(this->renderTexture2D.d);
	ClearBackground(ColorAlpha(RED, 0.0f));
}

void AlRtt::endRenderToTexture() {
	EndTextureMode();
}

void AlRtt::renderTexture() {
	BeginBlendMode(BLEND_ALPHA);

	// This will not be scaled if screen is resized
	// We have to change this->rect if we want the texture to be stretched according to raylib's fill
	// We have to change the source if we want the texture to be properly sized
	DrawTexturePro(this->renderTexture2D.d.texture,
		Rectangle{ 0.0f, 0.0f,
					 static_cast<float>(this->renderTexture2D.d.texture.width),
					 static_cast<float>(-this->renderTexture2D.d.texture.height) },
		this->actualDest,
		Vector2Zero(), 0.0f, WHITE);

	EndBlendMode();
}

Vector2 AlRtt::getLocalMousePos() {
	Vector2 worldMousePos = GetMousePosition();
	Vector2 resultPos = this->getWorldToLocal(worldMousePos);
	return resultPos;
}

Ray AlRtt::getMouseRay(Camera3D camera) {

	//    in case I get confused again
	//    Vector2 m = GetMousePosition();
	//    Vector2 worldPos = this->getLocalToWorld(mousePos);
	//    std::cout << "GetMousePosition(): " << m.x << "," << m.y << std::endl;
	//    std::cout << "localPos: " << mousePos.x << "," << mousePos.y << std::endl;
	//    std::cout << "worldPos: " << worldPos.x << "," << worldPos.y << std::endl;

		// Due to how GetMouseRay() function works, we need to scale the mouse coordinate to screen size
	Ray ray = GetMouseRay(this->getScaledLocalMousePos(), camera);

	//    std::cout << "scaledLocalMousePos: " << scaledLocalMousePos.x << "," << scaledLocalMousePos.y << std::endl;
	//    std::cout << std::endl;

	return ray;
}

Vector2 AlRtt::getScaledLocalMousePos() {
	Vector2 mousePos = this->getLocalMousePos();
	return Vector2{
			GetScreenWidth() / this->actualDest.width * mousePos.x,
			GetScreenHeight() / this->actualDest.height * mousePos.y };
}

// if screen is [800,600] and window is [400,300]
// then 200,100 will become 100,50
Vector2 AlRtt::getLocalToWorld(Vector2 localPos) {
	return {
			.x = this->actualDest.x + localPos.x,
			.y = this->actualDest.y + localPos.y,
	};
}

// if screen is [800,600] and window is [400,300]
// then 200,100 will become 400,200
// NOTE: the local coordinate will always be stretched to screen size because of how raylib works!
Vector2 AlRtt::getWorldToLocal(Vector2 worldPos) {
	return {
			.x = worldPos.x - this->actualDest.x,
			.y = worldPos.y - this->actualDest.y
	};
}


bool AlRtt::tryRecalculateRect() {
	if (this->needRecalculateActualDest || IsWindowResized()) {
		this->actualDest = Rectangle{
				.x = GetScreenWidth() * this->normalizedDest.x,
				.y = GetScreenHeight() * this->normalizedDest.y,
				.width = GetScreenWidth() * this->normalizedDest.width,
				.height = GetScreenHeight() * this->normalizedDest.height,
		};
		UnloadRenderTexture(this->renderTexture2D.d);
		this->renderTexture2D = LoadRenderTexture(this->actualDest.width, this->actualDest.height);
		this->needRecalculateActualDest = false;
		return true;
	}
	return false;
}
