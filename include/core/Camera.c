#include "Camera.h"

#include <SDL2/SDL_rect.h>

struct Camera
{
    Graphics *gfx;
    Vec2 m_position;
    Vec2 *m_follow;
    float m_centerRotation;
    SDL_Rect m_viewport;
};

Camera *CameraCreate(Graphics *gfx, Vec2 *follow)
{
    Camera *ret = (Camera *)SDL_malloc(sizeof(Camera));
    ret->gfx = gfx;
    ret->m_follow = follow;
    ret->m_centerRotation = 0.0f;
    ret->m_viewport = (SDL_Rect){0, 0, gfx->windowWidth, gfx->windowHeight};
    return ret;
}

void CameraDestroy(Camera *camera)
{
    SDL_free(camera);
}

void CameraUpdate(Camera *camera)
{
    camera->m_viewport = (SDL_Rect){0, 0, camera->gfx->windowWidth, camera->gfx->windowHeight};
    Vec2 *p = &camera->m_position;
    SDL_Rect *vp = &camera->m_viewport;
    Vec2 *f = camera->m_follow;
    if (f)
    {
        p->x = f->x - vp->w / 2.0f;
        p->y = f->y - vp->h / 2.0f;
    }
}

void CameraDraw(Camera *camera, Drawable drawable)
{
    drawable.dst.x -= camera->m_position.x;
    drawable.dst.y -= camera->m_position.y;
    if (SDL_HasIntersection(&drawable.dst, &camera->m_viewport))
        GraphicsDraw(camera->gfx, drawable);
}

Vec2 CameraMiddle(Camera *camera)
{
    SDL_Rect *vp = &camera->m_viewport;
    return (Vec2Create)((float)(vp->x + vp->w) / 2.0f + camera->m_position.x,
                        (float)(vp->y + vp->h) / 2.0f + camera->m_position.y);
}

void CameraAddRotation(Camera *camera, float ammount)
{
    camera->m_centerRotation += ammount;
}

void CameraSetFollow(Camera *camera, Vec2 *follow)
{
    camera->m_follow = follow;
}

Vec2 CameraGetPos(Camera *camera)
{
    return camera->m_position;
}

// Vec2 CameraRemoveRotation(Camera *camera, Vec2 vector)
// {
//     float oldX = vector.x;
//     vector.x = vector.x * cos(camera->m_centerRotation) - vector.y * sin(camera->m_centerRotation);
//     vector.y = oldX * sin(camera->m_centerRotation) + vector.y * cos(camera->m_centerRotation);

//     return vector;

//     //x' = x * cos(theta) - y * sin(theta)
//     //y' = x * sin(theta) + y * cos(theta)
// }