#include<raylib.h>
#include<raymath.h>
#include<string>
#include"../../arc_camera.h"
#include"../../pbr_shader.h"
#include"../../rlmath.h"
#include"../../object.h"

int main(int argc, char** argv) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	Vector2 windowSize = Vector2{ .x = 800, .y = 800 };
	InitWindow(windowSize.x, windowSize.y, "Hello PBR!");

	SetTargetFPS(60);

	Camera3D camera;
	camera.position = Vector3{ 2.0f, 4.0f, -10.0f };
	camera.target = Vector3Zero();
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	AlArcCameraInput alArcCameraInput;

	Model sphereModel = LoadModelFromMesh(GenMeshSphere(1.0f, 16, 16));
	std::shared_ptr<RlModel> testModel = std::make_shared<RlModel>(LoadModel("resources/KayKit_Adventurers_1.0_FREE/Characters/gltf/Barbarian.glb"));
	std::shared_ptr<RlTexture> testTexture = std::make_shared<RlTexture>(LoadTexture("resources/KayKit_Adventurers_1.0_FREE/Textures/barbarian_texture.png"));
	std::shared_ptr<AlPbrShader> pbrShader = std::make_shared<AlPbrShader>(LoadShader("resources/shaders/pbr.vert", "resources/shaders/pbr.frag"));
	
	AlObject object = AlObject(TransformOrigin(), testModel, pbrShader);
	object.pbrMaterial.albedo = ColorNormalize3(Color{0, 0, 0});
	object.pbrMaterial.metallic = 0.0f;
	object.pbrMaterial.roughness = 0.5f;
	object.pbrMaterial.AO = 1.0f;
	object.pbrMaterial.albedoMap = testTexture;
	object.pbrMaterial.needPassToMat = true;

	float lightPower = 500.0f;
	std::vector<AlLight> lights{
		AlLight{ .position = Vector3{-10.0f,  10.0f, 10.0f}, .colors = Vector3{lightPower, lightPower, lightPower} },
		AlLight{ .position = Vector3{10.0f,  10.0f, 10.0f}, .colors = Vector3{lightPower, lightPower, lightPower} },
		AlLight{ .position = Vector3{-10.0f,  -10.0f, 10.0f}, .colors = Vector3{lightPower, lightPower, lightPower} },
		AlLight{ .position = Vector3{-10.0f,  -10.0f, 10.0f}, .colors = Vector3{lightPower, lightPower, lightPower} },
	};

	float time = 0.0f;
	while (!WindowShouldClose()) {
		time += GetFrameTime();

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			HideCursor();
			alArcCameraInput.tryArcBall(&camera);
		}
		else {
			ShowCursor();
			alArcCameraInput.releaseArcBall();
		}
		alArcCameraInput.zoomOut(&camera);

		{
			BeginDrawing();

			DrawFPS(10, 10);
			DrawText("pbr demo!", 100, 100, 20, YELLOW);

			ClearBackground(ColorAlpha(SKYBLUE, 1.0f));
			BeginMode3D(camera);

			pbrShader->passScene(camera, &lights);

            for(int i = 0; i < lights.size(); ++i) {
                DrawModel(sphereModel, lights[i].position, 1.0, WHITE);
            }
			object.draw();

			DrawGrid(10, 1.0f);
			EndMode3D();
			EndDrawing();
		}
	}
;
	UnloadModel(sphereModel);
	CloseWindow();

	return 0;
}