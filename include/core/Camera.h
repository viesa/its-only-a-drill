#ifndef CAMERA_H
#define CAMERA_H

#include "Dependencies.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Clock.h"

void CameraInitialize();
void CameraUninitialize();

void CameraUpdate();
/// \param drawable: if drawable.dst is not within the
/// camera viewport, it will not be processed further
void CameraDraw(Drawable drawable);
void CameraDrawRect(SDL_Rect rect, SDL_Color color, SDL_bool filled);
void CameraDrawPoint(Vec2 pos, size_t radius);
void CameraDrawLine(int x1, int y1, int x2, int y2, SDL_Color color);

void CameraTransformRect(SDL_Rect *rect);
void CameraTransformPoint(Vec2 *point);

/// Return a position to the middle point of the viewport
Vec2 CameraMiddle();
void CameraAddRotation(float ammount);

/// Specifies which position should always be in
/// in the middle of the screen
/// The camera will be animated flying to target
void CameraSetFollow(Vec2 *follow);
/// Specifies which position should always be in
/// in the middle of the screen
/// The camera immediately snaps to target
void CameraSetFollowSnap(Vec2 *follow);

void CameraSetViewPort(SDL_Rect rect);

Vec2 CameraGetPos();

void CameraSetScale(float scale);

#endif