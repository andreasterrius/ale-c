#ifndef HELLO_C_GIZMO_H
#define HELLO_C_GIZMO_H

#include<raylib.h>
#include"rtt.h"

typedef enum AlGizmo_ModelType {
    ArrowX, ArrowY, ArrowZ,
    PlaneXY, PlaneXZ, PlaneYZ,
    RotationXY, RotationXZ, RotationYZ
} AlGizmo_ModelType;

typedef enum AlGizmo_ActiveAxis {
    X, Y, Z, XY, XZ, YZ
} AlGizmo_ActiveAxis;

typedef enum AleGizmo_Type {
    Translate, Scale, Rotate
} AlGizmo_Type;

static const float AlGizmo_MaximumDistanceScale = 1000.0f;

typedef struct AlGizmo_InitialClickInfo {
    bool exist;
    AlGizmo_ActiveAxis activeAxis;
    Vector3 position;
    // This is the position of the first initial click (only returns value for activeAxis)
    Vector3 firstRayPlaneHitPos;
    Vector3 initialSelfPos;
    // This is the position of last frame ray-plane intersection (only returns value for activeAxis)
    Vector3 lastFrameRayPlaneHitPos;
} AlGizmo_InitialClickInfo;

typedef struct AlGizmo_GrabAxis {
    RayCollision rayCollision;
    AlGizmo_ActiveAxis activeAxis;
} AlGizmo_GrabAxis;

typedef struct AlGizmo {
#define MODELS_LEN 9
    // Meshes
    Model models[MODELS_LEN];

    // State
    bool isHidden;
    Vector3 position;
    float scale;
    AlGizmo_Type gizmoType;

    // if not null, then user has selected one of the axis
    AlGizmo_InitialClickInfo initialClickInfo;
} AlGizmo;

void alGizmoInit(AlGizmo *self);

/// This usually happens when user press and holding left click (handled by caller)
/// This function is paired with release()
/// Return bool if it's holding something
bool alGizmoTryHold(AlGizmo *self, Transform *transform, Vector2 mousePos, Camera3D camera);

void alGizmoRelease(AlGizmo *self);

void alGizmoDeinit(AlGizmo *self);

void alGizmoRender(AlGizmo self);

static void alGizmoScaleAll(AlGizmo *self);

static AlGizmo_GrabAxis alGizmoGrabAxis(AlGizmo *self, Ray ray);

/// This returns the hit point position of ONLY the activeAxis (all else will be 0)
static RayCollision alGizmoRayPlaneIntersection(Ray ray, AlGizmo_ActiveAxis activeAxis, Vector3 planeCoord);

/// This will return the new position (not the delta)
/// I want it to snap to the where the mouse ray is intersecting the activeAxis
static Vector3 alGizmoHandleTranslate(AlGizmo *self, AlGizmo_ActiveAxis activeAxis, Vector3 rayPlaneHitPoint);

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
