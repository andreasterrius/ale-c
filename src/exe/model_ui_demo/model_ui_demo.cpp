#include<raylib.h>
#include<raymath.h>
#include<string>
#include"../../rtt.h"
#include"../../ui/model_ui.h"

int main(int argc, char** argv) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	Vector2 windowSize = Vector2{ .x = 800, .y = 800 };
	InitWindow(windowSize.x, windowSize.y, "Hello Model UI Panel!");

	SetTargetFPS(60);

	Camera3D camera;
	camera.position = Vector3{ 2.0f, 4.0f, -10.0f };
	camera.target = Vector3Zero();
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;


	Model model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
	std::shared_ptr<AlUnicodeFont> unicodeFont = std::make_shared<AlUnicodeFont>(
		"resources/font/NotoSansCJKjp-VF.ttf",
		24,
		std::vector{ AlUnicodeFontRange{.start = 0, .end = 255} }
	);
	Rectangle modelUiRect = Rectangle{ .x = 0.0, .y = 0.0, .width = 1.0, .height = 1.0 };
	
	AlModelUi modelUi(unicodeFont, modelUiRect, "resources/KayKit_Prototype_Bits_1.0_FREE/Assets/gltf");


	Ray ray = GetMouseRay(GetMousePosition(), camera);
	while (!WindowShouldClose()) {
		float dt = GetFrameTime();
		modelUi.tick(dt);

		modelUi.renderToTexture();

		{
			BeginDrawing();

			ClearBackground(ColorAlpha(SKYBLUE, 1.0f));
			modelUi.renderTexture();

			DrawFPS(10, 10);
			DrawText("model ui demo!", 100, 100, 20, YELLOW);
			//BeginMode3D(camera);

			//DrawRay(ray, RED);
			//DrawModel(model, Vector3Zero(), 1.0f, WHITE);


			//DrawGrid(10, 1.0f);
			//EndMode3D();

			EndDrawing();
		}
	}

	UnloadModel(model);
	CloseWindow();

	return 0;
}