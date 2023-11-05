//
// Created by alether on 9/26/23.
//
#include "gizmo.h"
#include "raymath.h"
#include <stddef.h>

void alGizmoInit(AlGizmo *self) {

    // load a flat shader here ?
    // the default raylib shader is flat though
    self->initialClickInfo = (AlGizmo_InitialClickInfo){0};
    self->scale = 1.0f;
    self->position = Vector3Zero();
    self->gizmoType = Translate;
    self->isHidden =  true;

    self->models[ArrowX] = LoadModel("resources/translate_gizmo/Arrow_X+.glb");
    self->models[ArrowY] = LoadModel("resources/translate_gizmo/Arrow_Y+.glb");
    self->models[ArrowZ] = LoadModel("resources/translate_gizmo/Arrow_Z+.glb");
    self->models[PlaneXY] = LoadModel("resources/translate_gizmo/Plane_XY.glb");
    self->models[PlaneXZ] = LoadModel("resources/translate_gizmo/Plane_XZ.glb");
    self->models[PlaneYZ] = LoadModel("resources/translate_gizmo/Plane_YZ.glb");
    self->models[RotationXY] = LoadModel("resources/translate_gizmo/Ring_XY.glb");
    self->models[RotationXZ] = LoadModel("resources/translate_gizmo/Ring_XZ.glb");
    self->models[RotationYZ] = LoadModel("resources/translate_gizmo/Ring_YZ.glb");
}

void alGizmoDeinit(AlGizmo *self) {
    for (int i = 0; i < MODELS_LEN; ++i) {
        UnloadModel(self->models[i]);
    }
}

bool alGizmoTryHold(AlGizmo *self, Transform *transform, Vector2 mousePos, Camera3D camera) {
    // There's no currently active selected object
    if (transform == NULL) {
        self->isHidden = true;
        return false;
    }

    // There's an active selected object
    // Show & set gizmo position properly
    self->isHidden = false;
    self->position = transform->translation;

    // TODO: This should not be here, this should be in tick(), but dependentPosition will be weakly owned.
    // Scale the size depending on the size
    float scale = Vector3DistanceSqr(camera.position, transform->translation) / AlGizmo_MaximumDistanceScale;
    scale = Clamp(scale, 0.25f, 1.0f);
    self->scale = scale;
    alGizmoScaleAll(self);

    // Try to check which arrow we're hitting
    Ray ray = GetMouseRay(mousePos, camera);

    if (!self->initialClickInfo.exist) {
        // This is initial click on arrow/plane in gizmo, let's save the initial clickInfo
        AlGizmo_GrabAxis grabAxis = alGizmoGrabAxis(self, ray);

        if (!grabAxis.rayCollision.hit) { // no arrows were clicked
            return false;
        }

        // Get a ray to plane intersection.
        RayCollision rayPlaneHit = alGizmoRayPlaneIntersection(ray, grabAxis.activeAxis, self->position);
        if (!rayPlaneHit.hit) {
            return false;
        }

        self->initialClickInfo = (AlGizmo_InitialClickInfo) {
                .exist = true,
                .activeAxis = grabAxis.activeAxis,
                .position = grabAxis.rayCollision.point,
                .firstRayPlaneHitPos = rayPlaneHit.point,
                .initialSelfPos = self->position,
                .lastFrameRayPlaneHitPos = rayPlaneHit.point,
        };
        return true;
    }

    // This is no longer initial hit, but is a dragging movement
    RayCollision rayPlaneHit = alGizmoRayPlaneIntersection(ray, self->initialClickInfo.activeAxis, self->position);

    // Ignore ray-plane parallel cases
    if (rayPlaneHit.hit) {
        if(self->gizmoType == Translate) {
            Vector3 newPos = alGizmoHandleTranslate(self, self->initialClickInfo.activeAxis, rayPlaneHit.point);
            self->position = newPos;
            transform->translation = newPos;
        } else if(self->gizmoType == Scale) {
            // unlike translate, we just return delta here
            Vector3 delta = Vector3Subtract(rayPlaneHit.point, self->initialClickInfo.lastFrameRayPlaneHitPos);
            transform->scale = Vector3Add(transform->scale, delta);
        } else if(self->gizmoType == Rotate) {
            Vector3 unitVecA = Vector3Subtract(rayPlaneHit.point, self->position);
            Vector3 unitVecB = Vector3Subtract(self->initialClickInfo.lastFrameRayPlaneHitPos, self->position);

            Vector4 delta = QuaternionFromVector3ToVector3(unitVecB, unitVecA);
            //not commutative, multiply order matters!
            // world rotation
            transform->rotation = QuaternionMultiply(delta, transform->rotation);
            // local rotation : QuaternionMultiply(transform->rotation, delta);
        }
        self->initialClickInfo.lastFrameRayPlaneHitPos = rayPlaneHit.point;
    }

    return true;
}

void alGizmoRelease(AlGizmo *self) {
    self->initialClickInfo = (AlGizmo_InitialClickInfo) {0};
}

void alGizmoScaleAll(AlGizmo *self) {
    self->models[ArrowX].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[ArrowY].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[ArrowZ].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[PlaneXY].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[PlaneXZ].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[PlaneYZ].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[RotationXY].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[RotationXZ].transform = MatrixScale(self->scale, self->scale, self->scale);
    self->models[RotationYZ].transform = MatrixScale(self->scale, self->scale, self->scale);
}

AlGizmo_GrabAxis alGizmoGrabAxis(AlGizmo *self, Ray ray) {
    Matrix transformTranslate = MatrixTranslate(self->position.x, self->position.y, self->position.z);
    Matrix transformScale = MatrixScale(self->scale, self->scale, self->scale);
    Matrix transform = MatrixMultiply(transformScale, transformTranslate);

    if (self->gizmoType == Translate || self->gizmoType == Scale) {
        RayCollision coll = GetRayCollisionMesh(ray, self->models[ArrowX].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = X};
        }
        coll = GetRayCollisionMesh(ray, self->models[ArrowY].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = Y};
        }
        coll = GetRayCollisionMesh(ray, self->models[ArrowZ].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = Z};
        }
        coll = GetRayCollisionMesh(ray, self->models[PlaneYZ].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = YZ};
        }
        coll = GetRayCollisionMesh(ray, self->models[PlaneXZ].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = XZ};
        }
        coll = GetRayCollisionMesh(ray, self->models[PlaneXY].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = XY};
        }
    } else if (self->gizmoType == Rotate) {
        RayCollision coll = GetRayCollisionMesh(ray, self->models[RotationYZ].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = YZ};
        }
        coll = GetRayCollisionMesh(ray, self->models[RotationXZ].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = XZ};
        }
        coll = GetRayCollisionMesh(ray, self->models[RotationXY].meshes[0], transform);
        if (coll.hit) {
            return (AlGizmo_GrabAxis) { .rayCollision = coll, .activeAxis = XY};
        }
    }

    return (AlGizmo_GrabAxis) {
        .rayCollision = (RayCollision) {
            .hit = false
        }
    };
}

RayCollision alGizmoRayPlaneIntersection(Ray ray, AlGizmo_ActiveAxis activeAxis, Vector3 planeCoord) {

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

void alGizmoRender(const AlGizmo self) {
    if (self.isHidden) {
        return;
    }

    // Must be called inside BeginMode3D render
    if (self.gizmoType == Translate || self.gizmoType == Scale) {
        DrawModel(self.models[ArrowX], self.position, 1.0f, RED);
        DrawModel(self.models[ArrowY], self.position, 1.0f, GREEN);
        DrawModel(self.models[ArrowZ], self.position, 1.0f, BLUE);
        DrawModel(self.models[PlaneXY], self.position, 1.0f, BLUE);
        DrawModel(self.models[PlaneXZ], self.position, 1.0f, GREEN);
        DrawModel(self.models[PlaneYZ], self.position, 1.0f, RED);
    } else if (self.gizmoType == Rotate) {
        DrawModel(self.models[RotationXY], self.position, 1.0f, BLUE);
        DrawModel(self.models[RotationXZ], self.position, 1.0f, GREEN);
        DrawModel(self.models[RotationYZ], self.position, 1.0f, RED);
    }
}

Vector3 alGizmoHandleTranslate(AlGizmo *self, AlGizmo_ActiveAxis activeAxis, Vector3 rayPlaneHitPoint) {
    Vector3 initialRayPos = Vector3Zero();
    if (activeAxis == X) {
        initialRayPos.x =
                self->initialClickInfo.firstRayPlaneHitPos.x - self->initialClickInfo.initialSelfPos.x;
    } else if (activeAxis == Y) {
        initialRayPos.y =
                self->initialClickInfo.firstRayPlaneHitPos.y - self->initialClickInfo.initialSelfPos.y;
    } else if (activeAxis == Z) {
        initialRayPos.z =
                self->initialClickInfo.firstRayPlaneHitPos.z - self->initialClickInfo.initialSelfPos.z;
    } else if (activeAxis == XY) {
        initialRayPos.x =
                self->initialClickInfo.firstRayPlaneHitPos.x - self->initialClickInfo.initialSelfPos.x;
        initialRayPos.y =
                self->initialClickInfo.firstRayPlaneHitPos.y - self->initialClickInfo.initialSelfPos.y;
    } else if (activeAxis == YZ) {
        initialRayPos.z =
                self->initialClickInfo.firstRayPlaneHitPos.z - self->initialClickInfo.initialSelfPos.z;
        initialRayPos.y =
                self->initialClickInfo.firstRayPlaneHitPos.y - self->initialClickInfo.initialSelfPos.y;
    } else if (activeAxis == XZ) {
        initialRayPos.x =
                self->initialClickInfo.firstRayPlaneHitPos.x - self->initialClickInfo.initialSelfPos.x;
        initialRayPos.z =
                self->initialClickInfo.firstRayPlaneHitPos.z - self->initialClickInfo.initialSelfPos.z;
    }

    return Vector3Subtract(rayPlaneHitPoint, initialRayPos);
}
