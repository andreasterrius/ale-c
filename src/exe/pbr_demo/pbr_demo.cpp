#include<raylib.h>
#include<raymath.h>
#include<string>
#include"../../arc_camera.h"

class Light {
public:

};

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

	Model model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
	Model sphereModel = LoadModelFromMesh(GenMeshSphere(1.0f, 16, 16));
	//Model testModel = LoadModel("resources/starter_assets/OBJ/SM_Bean_Cop_01.obj");
	Model testModel = LoadModel("resources/KayKit_Adventurers_1.0_FREE/Characters/gltf/Barbarian.glb");

	Shader pbrShader = LoadShader("resources/shaders/pbr.vert", "resources/shaders/pbr.frag");
	testModel.materials[0].shader = pbrShader;
	testModel.materials[1].shader = pbrShader;


	float color[3] = { 0.5, 0.0, 0.0 };
	float value = 0.5f;
	float metallic = 0.2f;
	SetShaderValue(pbrShader, GetShaderLocation(pbrShader, "albedo"), color, SHADER_UNIFORM_VEC3);
	SetShaderValue(pbrShader, GetShaderLocation(pbrShader, "metallic"), &metallic, SHADER_UNIFORM_FLOAT);
	SetShaderValue(pbrShader, GetShaderLocation(pbrShader, "roughness"), &value, SHADER_UNIFORM_FLOAT);
	SetShaderValue(pbrShader, GetShaderLocation(pbrShader, "ao"), &value, SHADER_UNIFORM_FLOAT);
	

	Vector3 lightPositions[] = {
		Vector3(-10.0f,  10.0f, 10.0f),
		Vector3(10.0f,  10.0f, 10.0f),
		Vector3(-10.0f, -10.0f, 10.0f),
		Vector3(10.0f, -10.0f, 10.0f),
	};
	Vector3 lightColors[] = {
		Vector3(300.0f, 300.0f, 300.0f),
		Vector3(300.0f, 300.0f, 300.0f),
		Vector3(300.0f, 300.0f, 300.0f),
		Vector3(300.0f, 300.0f, 300.0f)
	};
	int lightPositionLoc[] = {
		GetShaderLocation(pbrShader, "lightPositions[0]"),
		GetShaderLocation(pbrShader, "lightPositions[1]"),
		GetShaderLocation(pbrShader, "lightPositions[2]"),
		GetShaderLocation(pbrShader, "lightPositions[3]"),
	};
	int lightColorLoc[] = {
		GetShaderLocation(pbrShader, "lightColors[0]"),
		GetShaderLocation(pbrShader, "lightColors[1]"),
		GetShaderLocation(pbrShader, "lightColors[2]"),
		GetShaderLocation(pbrShader, "lightColors[3]"),
	};
	int camPosLoc = GetShaderLocation(pbrShader, "camPos");
	
	Ray ray = GetMouseRay(GetMousePosition(), camera);
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

			SetShaderValue(pbrShader, camPosLoc, &camera.position, SHADER_UNIFORM_VEC3);
			
			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				Vector3 newPos = Vector3Add(lightPositions[i], Vector3(sin(time * 5.0) * 5.0, 0.0, 0.0));
			
				SetShaderValue(pbrShader, lightPositionLoc[i], &newPos, SHADER_UNIFORM_VEC3);
				SetShaderValue(pbrShader, lightColorLoc[i], &lightColors[i], SHADER_UNIFORM_VEC3);

				//model = glm::mat4(1.0f);
				//model = glm::translate(model, newPos);
				//model = glm::scale(model, glm::vec3(0.5f));
				//shader.setMat4("model", model);
				//shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				DrawModel(sphereModel, newPos, 1.0f, WHITE);
			}


			DrawRay(ray, RED);
			DrawModel(testModel, Vector3Zero(), 1.0f, WHITE);

			DrawGrid(10, 1.0f);
			EndMode3D();
			EndDrawing();
		}
	}

	UnloadModel(model);
	UnloadModel(sphereModel);
	CloseWindow();

	return 0;
}