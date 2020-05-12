#include "Camera.h"

struct Camera
{
    Graphics *gfx;
    Vec2 position;
    Vec2 *follow;
    float centerRotation;
    SDL_Rect viewport;
    float scale;

    SDL_bool newTarget;
};

Camera *CameraCreate(Graphics *gfx, Vec2 *follow)
{
    Camera *ret = MALLOC(Camera);
    ret->gfx = gfx;
    ret->follow = follow;
    ret->centerRotation = 0.0f;
    ret->viewport = (SDL_Rect){0, 0, gfx->window->width, gfx->window->height};
    ret->scale = 0.0f;
    ret->newTarget = SDL_FALSE;
    return ret;
}

void CameraDestroy(Camera *camera)
{
    SDL_free(camera);
}

void CameraUpdate(Camera *camera)
{
    Vec2 *p = &camera->position;
    SDL_Rect *vp = &camera->viewport;
    Vec2 *f = camera->follow;
    if (f)
    {
        if (camera->newTarget)
        {
            Vec2 desired = Vec2Create(f->x - (vp->x + vp->w / 2.0f), f->y - (vp->y + vp->h / 2.0f));
            camera->position = Vec2Add(camera->position, Vec2MulL(Vec2Sub(desired, camera->position), 0.2f));
            if (Vec2Len(Vec2Add(camera->position, desired)) < 2.0f)
                camera->newTarget = SDL_FALSE;
        }
        else
        {
            p->x = f->x - vp->w / 2.0f;
            p->y = f->y - vp->h / 2.0f;
        }
    }
}

void CameraDraw(Camera *camera, Drawable drawable)
{
    drawable.dst.x -= camera->position.x;
    drawable.dst.y -= camera->position.y;
    Vec2 mid = CameraMiddle(camera);
    drawable.dst.x -= mid.x;
    drawable.dst.y -= mid.y;
    drawable.dst.x *= camera->scale;
    drawable.dst.y *= camera->scale;
    drawable.dst.w *= camera->scale;
    drawable.dst.h *= camera->scale;
    drawable.dst.x += mid.x;
    drawable.dst.y += mid.y;
    if (SDL_HasIntersection(&drawable.dst, &camera->viewport))
        GraphicsDraw(camera->gfx, drawable);
}

Vec2 CameraMiddle(Camera *camera)
{
    SDL_Rect *vp = &camera->viewport;
    return (Vec2Create)((float)vp->x + (float)vp->w / 2.0f,
                        (float)vp->y + (float)vp->h / 2.0f);
}

void CameraAddRotation(Camera *camera, float ammount)
{
    camera->centerRotation += ammount;
}

void CameraSetFollow(Camera *camera, Vec2 *follow)
{
    camera->follow = follow;
    camera->newTarget = SDL_TRUE;
}

void CameraSetViewPort(Camera *camera, SDL_Rect rect)
{
    camera->viewport = rect;
}

Vec2 CameraGetPos(Camera *camera)
{
    return camera->position;
}

void CameraSetScale(Camera *camera, float scale)
{
    camera->scale = scale;
}
