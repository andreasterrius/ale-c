#ifndef HELLO_C_GIZMO_H
#define HELLO_C_GIZMO_H

#include<raylib.h>

typedef enum AleGizmo_ModelType {
    ArrowX, ArrowY, ArrowZ,
    PlaneXY, PlaneXZ, PlaneYZ,
    RotationXY, RotationXZ, RotationYZ
} AleGizmo_ModelType;

typedef enum AleGizmo_ActiveAxis {
    X, Y, Z, XY, XZ, YZ
} AleGizmo_ActiveAxis;

typedef enum AleGizmo_Type {
    Translate, Scale, Rotate
} AleGizmo_Type;

static const float AleGizmo_MaximumDistanceScale = 1000.0f;

typedef struct AleGizmo_InitialClickInfo {
    bool exist;
    AleGizmo_ActiveAxis activeAxis;
    Vector3 position;
    // This is the position of the first initial click (only returns value for activeAxis)
    Vector3 firstRayPlaneHitPos;
    Vector3 initialSelfPos;
    // This is the position of last frame ray-plane intersection (only returns value for activeAxis)
    Vector3 lastFrameRayPlaneHitPos;
} AleGizmo_InitialClickInfo;

typedef struct AleGizmo_GrabAxis {
    RayCollision rayCollision;
    AleGizmo_ActiveAxis activeAxis;
} AleGizmo_GrabAxis;

typedef struct AleGizmo {
#define MODELS_LEN 9
    // Meshes
    Model models[MODELS_LEN];

    // State
    bool isHidden;
    Vector3 position;
    float scale;
    AleGizmo_Type gizmoType;

    // if not null, then user has selected one of the axis
    AleGizmo_InitialClickInfo initialClickInfo;
} AleGizmo;

void alGizmoInit(AleGizmo *self);

/// This usually happens when user press and holding left click (handled by caller)
/// This function is paired with release()
void alGizmoTryHold(AleGizmo *self, Transform *transform, Camera3D camera);

void alGizmoRelease(AleGizmo *self);

void alGizmoDeinit(AleGizmo *self);

void alGizmoRender(AleGizmo self);

static void alGizmoScaleAll(AleGizmo *self);

static AleGizmo_GrabAxis alGizmoGrabAxis(AleGizmo *self, Ray ray);

/// This returns the hit point position of ONLY the activeAxis (all else will be 0)
static RayCollision alGizmoRayPlaneIntersection(Ray ray, AleGizmo_ActiveAxis activeAxis, Vector3 planeCoord);

/// This will return the new position (not the delta)
/// I want it to snap to the where the mouse ray is intersecting the activeAxis
static Vector3 alGizmoHandleTranslate(AleGizmo *self, AleGizmo_ActiveAxis activeAxis, Vector3 rayPlaneHitPoint);

/// Unlike translation, this will return the delta instead
//static void alGizmoGetRayplaneHitPosDelta(AleGizmo *self, AleGizmo_ActiveAxis activeAxis, Vector3 rayPlaneHitPoint, Vector3 );
//
//static void alGizmoHandleRotate() {
////    if (activeAxis == XY) {
////        initialRayPos.x =
////                self->initialClickInfo.firstRayPlaneHitPos.x - self->initialClickInfo.initialSelfPos.x;
////        initialRayPos.y =
////                self->initialClickInfo.firstRayPlaneHitPos.y - self->initialClickInfo.initialSelfPos.y;
////    } else if (activeAxis == YZ) {
////        initialRayPos.z =
////                self->initialClickInfo.firstRayPlaneHitPos.z - self->initialClickInfo.initialSelfPos.z;
////        initialRayPos.y =
////                self->initialClickInfo.firstRayPlaneHitPos.y - self->initialClickInfo.initialSelfPos.y;
////    } else if (activeAxis == XZ) {
////        initialRayPos.x =
////                self->initialClickInfo.firstRayPlaneHitPos.x - self->initialClickInfo.initialSelfPos.x;
////        initialRayPos.z =
////                self->initialClickInfo.firstRayPlaneHitPos.z - self->initialClickInfo.initialSelfPos.z;
////    }
//}

#endif //HELLO_C_GIZMO_H
