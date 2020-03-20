#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Vec2.h"
#include "Graphics.h"

typedef struct Camera Camera;

Camera *CameraCreate(Graphics *gfx, Vec2 *follow);
void CameraDestroy(Camera *camera);

void CameraUpdate(Camera *camera);
void CameraDraw(Camera *camera, Drawable drawable);
void PlayerDraw(Camera *camera, Drawable drawable);

SDL_Point CameraMiddle(Camera *camera);
void CameraAddRotation(Camera *camera, float ammount);

void CameraSetFollow(Camera *camera, Vec2 *follow);

#endif