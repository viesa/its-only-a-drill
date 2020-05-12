#ifndef CAMERA_H
#define CAMERA_H

#include "Dependencies.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Clock.h"

typedef struct Camera Camera;

Camera *CameraCreate(Graphics *gfx, Vec2 *follow);
void CameraDestroy(Camera *camera);

void CameraUpdate(Camera *camera);
/// \param drawable: if drawable.dst is not within the
/// camera viewport, it will not be processed further
void CameraDraw(Camera *camera, Drawable drawable);

/// Return a position to the middle point of the viewport
Vec2 CameraMiddle(Camera *camera);
void CameraAddRotation(Camera *camera, float ammount);

/// Specifies which position should always be in
/// in the middle of the screen
/// The camera will be animated flying to target
void CameraSetFollow(Camera *camera, Vec2 *follow);
/// Specifies which position should always be in
/// in the middle of the screen
/// The camera immediately snaps to target
void CameraSetFollowSnap(Camera *camera, Vec2 *follow);

void CameraSetViewPort(Camera *camera, SDL_Rect rect);

Vec2 CameraGetPos(Camera *camera);

void CameraSetScale(Camera *camera, float scale);

#endif