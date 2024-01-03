#include"model_preview.h"
#include"../rlmath.h"

AlModelPreview::AlModelPreview(std::shared_ptr<RlModel> model) : model(model), 
    displayTexture(FullNormRectOrigin()) {
    BoundingBox bb = GetModelBoundingBox(model->d);
    Vector3 mid = Vector3{
        .x = (bb.min.x + bb.max.x) / 2.0f,
        .y = (bb.min.y + bb.max.y) / 2.0f,
        .z = (bb.min.z + bb.max.z) / 2.0f,
    };

    this->camera = Camera{
        .position = Vector3{ 1.0f, 2.0f, -5.0f },
        .target = Vector3Zero(),
        .up = Vector3{0.0f, 1.0f, 0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };
}

void AlModelPreview::renderToTexture() {
    if (this->displayTextureRenderedOnce) return;
   
    this->displayTexture.beginRenderToTexture();

    ClearBackground(ColorAlpha(SKYBLUE, 1.0f));
    BeginMode3D(camera);
    DrawModel(this->model->d, Vector3Zero(), 1.0f, WHITE);
    DrawGrid(10, 1.0f);
    EndMode3D();

	this->displayTexture.endRenderToTexture();
    this->displayTextureRenderedOnce = true;
}

void AlModelPreview::renderTexture(){
    this->displayTexture.renderTexture();
}

void AlModelPreview::setRect(Rectangle rect) {
    this->displayTexture.actualDest = rect;
    this->displayTexture.needRecalculateActualDest = false;
}