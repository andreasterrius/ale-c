//
// Created by alether on 9/26/23.
//
#include "gizmo.h"
#include "raymath.h"

AlGizmo::AlGizmo() {

    // load a flat shader here ?
    // the default raylib shader is flat though
    this->initialClickInfo = (AlGizmo_InitialClickInfo) {0};
    this->scale = 1.0f;
    this->position = Vector3Zero();
    this->gizmoType = Translate;
    this->isHidden = true;

    this->models[ArrowX] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Arrow_X+.glb"));
    this->models[ArrowY] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Arrow_Y+.glb"));
    this->models[ArrowZ] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Arrow_Z+.glb"));
    this->models[PlaneXY] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Plane_XY.glb"));
    this->models[PlaneXZ] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Plane_XZ.glb"));
    this->models[PlaneYZ] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Plane_YZ.glb"));
    this->models[RotationXY] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Ring_XY.glb"));
    this->models[RotationXZ] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Ring_XZ.glb"));
    this->models[RotationYZ] = std::make_unique<Model>(LoadModel("resources/translate_gizmo/Ring_YZ.glb"));
}

AlGizmo::~AlGizmo() {
    for (int i = 0; i < MODELS_LEN; ++i) {
        UnloadModel(*this->models[i]);
    }
}

bool AlGizmo::tryHold(Transform *transform, Vector2 mousePos, Camera3D camera) {
    // There's no currently active selected object
    if (transform == NULL) {
        this->isHidden = true;
        return false;
    }

    // There's an active selected object
    // Show & set gizmo position properly
    this->isHidden = false;
    this->position = transform->translation;

    // TODO: This should not be here, this should be in tick(), but dependentPosition will be weakly owned.
    // Scale the size depending on the size
    float scale = Vector3DistanceSqr(camera.position, transform->translation) / AlGizmo_MaximumDistanceScale;
    scale = Clamp(scale, 0.25f, 1.0f);
    this->scale = scale;
    this->scaleAll();

    // Try to check which arrow we're hitting
    Ray ray = GetMouseRay(mousePos, camera);

    if (!this->initialClickInfo.exist) {
        // This is initial click on arrow/plane in gizmo, let's save the initial clickInfo
        AlGizmo_GrabAxis grabAxis = this->grabAxis(ray);

        if (!grabAxis.rayCollision.hit) { // no arrows were clicked
            return false;
        }

        // Get a ray to plane intersection.
        RayCollision rayPlaneHit = this->rayPlaneIntersection(ray, grabAxis.activeAxis, this->position);
        if (!rayPlaneHit.hit) {
            return false;
        }

        this->initialClickInfo = (AlGizmo_InitialClickInfo) {
                .exist = true,
                .activeAxis = grabAxis.activeAxis,
                .position = grabAxis.rayCollision.point,
                .firstRayPlaneHitPos = rayPlaneHit.point,
                .initialSelfPos = this->position,
                .lastFrameRayPlaneHitPos = rayPlaneHit.point,
        };
        return true;
    }

    // This is no longer initial hit, but is a dragging movement
    RayCollision rayPlaneHit = this->rayPlaneIntersection(ray, this->initialClickInfo.activeAxis, this->position);

    // Ignore ray-plane parallel cases
    if (rayPlaneHit.hit) {
        if (this->gizmoType == Translate) {
            Vector3 newPos = this->handleTranslate(this->initialClickInfo.activeAxis, rayPlaneHit.point);
            this->position = newPos;
            transform->translation = newPos;
        } else if (this->gizmoType == Scale) {
            // unlike translate, we just return delta here
            Vector3 delta = Vector3Subtract(rayPlaneHit.point, this->initialClickInfo.lastFrameRayPlaneHitPos);
            transform->scale = Vector3Add(transform->scale, delta);
        } else if (this->gizmoType == Rotate) {
            Vector3 unitVecA = Vector3Subtract(rayPlaneHit.point, this->position);
            Vector3 unitVecB = Vector3Subtract(this->initialClickInfo.lastFrameRayPlaneHitPos, this->position);

            Vector4 delta = QuaternionFromVector3ToVector3(unitVecB, unitVecA);
            //not commutative, multiply order matters!
            // world rotation
            transform->rotation = QuaternionMultiply(delta, transform->rotation);
            // local rotation : QuaternionMultiply(transform->rotation, delta);
        }
        this->initialClickInfo.lastFrameRayPlaneHitPos = rayPlaneHit.point;
    }

    return true;
}

void AlGizmo::release() {
    this->initialClickInfo = (AlGizmo_InitialClickInfo) {0};
}

void AlGizmo::scaleAll() {
    this->models[ArrowX]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[ArrowY]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[ArrowZ]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[PlaneXY]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[PlaneXZ]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[PlaneYZ]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[RotationXY]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[RotationXZ]->transform = MatrixScale(this->scale, this->scale, this->scale);
    this->models[RotationYZ]->transform = MatrixScale(this->scale, this->scale, this->scale);
}

AlGizmo_GrabAxis AlGizmo::grabAxis(Ray ray) {
    Matrix transformTranslate = MatrixTranslate(this->position.x, this->position.y, this->position.z);
    Matrix transformScale = MatrixScale(this->scale, this->scale, this->scale);
    Matrix transform = MatrixMultiply(transformScale, transformTranslate);

    if (this->gizmoType == Translate || this->gizmoType == Scale) {
        RayCollision coll = GetRayCollisionMesh(ray, this->models[ArrowX]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = X
            };
        }
        coll = GetRayCollisionMesh(ray, this->models[ArrowY]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = Y
            };
        }
        coll = GetRayCollisionMesh(ray, this->models[ArrowZ]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = Z
            };
        }
        coll = GetRayCollisionMesh(ray, this->models[PlaneYZ]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = YZ
            };
        }
        coll = GetRayCollisionMesh(ray, this->models[PlaneXZ]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = XZ
            };
        }
        coll = GetRayCollisionMesh(ray, this->models[PlaneXY]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = XY
            };
        }
    } else if (this->gizmoType == Rotate) {
        RayCollision coll = GetRayCollisionMesh(ray, this->models[RotationYZ]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = YZ
            };
        }
        coll = GetRayCollisionMesh(ray, this->models[RotationXZ]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = XZ
            };
        }
        coll = GetRayCollisionMesh(ray, this->models[RotationXY]->meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) {
                    .rayCollision = coll,
                    .activeAxis = XY
            };
        }
    }

    return (AlGizmo_GrabAxis) {
                .rayCollision = (RayCollision) {
                .hit = false
            }
    };
}

RayCollision AlGizmo::rayPlaneIntersection(Ray ray, AlGizmo_ActiveAxis activeAxis, Vector3 planeCoord) {

    Vector3 activeAxisDir;
    float t = 0.0f;
    if (activeAxis == X || activeAxis == XZ) {
        activeAxisDir = (Vector3) {1.0f, 0.0f, 0.0f};
        t = (planeCoord.y - ray.position.y) / ray.direction.y;
    } else if (activeAxis == Y || activeAxis == XY) {
        activeAxisDir = (Vector3) {0.0f, 1.0f, 0.0f};
        t = (planeCoord.z - ray.position.z) / ray.direction.z;
    } else if (activeAxis == Z || activeAxis == YZ) {
        activeAxisDir = (Vector3) {0.0f, 0.0f, 1.0f};
        t = (planeCoord.x - ray.position.x) / ray.direction.x;
    } else {
        return (RayCollision) {
                .hit = false,
                .distance = 0.0f,
                .point = Vector3Zero(),
                .normal = Vector3Zero(),
        };
    }
    float isNearParallel = Vector3DotProduct(ray.direction, activeAxisDir);

    if (isNearParallel < -0.99 || isNearParallel > 0.99) {
        return (RayCollision) {
                .hit = false,
                .distance = 0.0f,
                .point = Vector3Zero(),
                .normal = Vector3Zero(),
        };
    }

    Vector3 intersectionCoord = planeCoord;
    if (activeAxis == X) {
        intersectionCoord.x = ray.position.x + t * ray.direction.x;
    } else if (activeAxis == Y) {
        intersectionCoord.y = ray.position.y + t * ray.direction.y;
    } else if (activeAxis == Z) {
        intersectionCoord.z = ray.position.z + t * ray.direction.z;
    } else if (activeAxis == XY) {
        intersectionCoord.x = ray.position.x + t * ray.direction.x;
        intersectionCoord.y = ray.position.y + t * ray.direction.y;
    } else if (activeAxis == YZ) {
        intersectionCoord.z = ray.position.z + t * ray.direction.z;
        intersectionCoord.y = ray.position.y + t * ray.direction.y;
    } else if (activeAxis == XZ) {
        intersectionCoord.x = ray.position.x + t * ray.direction.x;
        intersectionCoord.z = ray.position.z + t * ray.direction.z;
    }

    return (RayCollision) {
            .hit = true,
            .point = intersectionCoord,
            .distance = 0.0f,
            .normal = Vector3Zero(),
    };
}

void AlGizmo::render() {
    if (this->isHidden) {
        return;
    }

    // Must be called inside BeginMode3D render
    if (this->gizmoType == Translate || this->gizmoType == Scale) {
        DrawModel(*this->models[ArrowX], this->position, 1.0f, RED);
        DrawModel(*this->models[ArrowY], this->position, 1.0f, GREEN);
        DrawModel(*this->models[ArrowZ], this->position, 1.0f, BLUE);
        DrawModel(*this->models[PlaneXY], this->position, 1.0f, BLUE);
        DrawModel(*this->models[PlaneXZ], this->position, 1.0f, GREEN);
        DrawModel(*this->models[PlaneYZ], this->position, 1.0f, RED);
    } else if (this->gizmoType == Rotate) {
        DrawModel(*this->models[RotationXY], this->position, 1.0f, BLUE);
        DrawModel(*this->models[RotationXZ], this->position, 1.0f, GREEN);
        DrawModel(*this->models[RotationYZ], this->position, 1.0f, RED);
    }
}

Vector3 AlGizmo::handleTranslate(AlGizmo_ActiveAxis activeAxis, Vector3 rayPlaneHitPoint) {
    Vector3 initialRayPos = Vector3Zero();
    if (activeAxis == X) {
        initialRayPos.x =
                this->initialClickInfo.firstRayPlaneHitPos.x - this->initialClickInfo.initialSelfPos.x;
    } else if (activeAxis == Y) {
        initialRayPos.y =
                this->initialClickInfo.firstRayPlaneHitPos.y - this->initialClickInfo.initialSelfPos.y;
    } else if (activeAxis == Z) {
        initialRayPos.z =
                this->initialClickInfo.firstRayPlaneHitPos.z - this->initialClickInfo.initialSelfPos.z;
    } else if (activeAxis == XY) {
        initialRayPos.x =
                this->initialClickInfo.firstRayPlaneHitPos.x - this->initialClickInfo.initialSelfPos.x;
        initialRayPos.y =
                this->initialClickInfo.firstRayPlaneHitPos.y - this->initialClickInfo.initialSelfPos.y;
    } else if (activeAxis == YZ) {
        initialRayPos.z =
                this->initialClickInfo.firstRayPlaneHitPos.z - this->initialClickInfo.initialSelfPos.z;
        initialRayPos.y =
                this->initialClickInfo.firstRayPlaneHitPos.y - this->initialClickInfo.initialSelfPos.y;
    } else if (activeAxis == XZ) {
        initialRayPos.x =
                this->initialClickInfo.firstRayPlaneHitPos.x - this->initialClickInfo.initialSelfPos.x;
        initialRayPos.z =
                this->initialClickInfo.firstRayPlaneHitPos.z - this->initialClickInfo.initialSelfPos.z;
    }

    return Vector3Subtract(rayPlaneHitPoint, initialRayPos);
}
