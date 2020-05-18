#include "Camera.h"

typedef struct Camera
{
    Vec2 position;
    Vec2 *follow;
    float centerRotation;
    SDL_Rect viewport;
    float scale;

    SDL_bool newTarget;
} Camera;

static Camera *camera;

void CameraInitialize()
{
    camera = MALLOC(Camera);
    ALLOC_ERROR_CHECK(camera);
    camera->follow = NULL;
    camera->centerRotation = 0.0f;
    camera->viewport = (SDL_Rect){0, 0, WindowGetWidth(), WindowGetHeight()};
    camera->scale = 0.0f;
    camera->newTarget = SDL_FALSE;
}

void CameraUninitialize()
{
    SDL_free(camera);
}

void CameraUpdate()
{
    Vec2 *p = &camera->position;
    SDL_Rect *vp = &camera->viewport;
    Vec2 *f = camera->follow;
    if (f)
    {
        if (camera->newTarget)
        {
            Vec2 desired = Vec2Create(f->x - (vp->x + vp->w / 2.0f), f->y - (vp->y + vp->h / 2.0f));
            Vec2 towards = Vec2MulL(Vec2Sub(desired, camera->position), 0.2f * ClockGetDeltaTime() * 30.0f);
            if (Vec2Len(towards) < 5.0f)
            {
                towards = Vec2MulL(Vec2Unit(towards), 5.0f);
            }
            if (Vec2Len(towards) > Vec2Len(Vec2Sub(camera->position, desired)))
            {
                towards = Vec2MulL(Vec2Unit(towards), Vec2Len(Vec2Sub(camera->position, desired)));
            }
            camera->position = Vec2Add(camera->position, towards);
            if (Vec2Len(Vec2Sub(camera->position, desired)) < 3.0f)
                camera->newTarget = SDL_FALSE;
        }
        else
        {
            p->x = f->x - (vp->x + vp->w / 2.0f);
            p->y = f->y - (vp->y + vp->h / 2.0f);
        }
    }
}

void CameraDraw(Drawable drawable)
{
    CameraTransformRect(&drawable.dst);
    if (SDL_HasIntersection(&drawable.dst, &camera->viewport))
        GraphicsDraw(drawable);
}

void CameraDrawRect(SDL_Rect rect, SDL_Color color, SDL_bool filled)
{
    CameraTransformRect(&rect);
    if (SDL_HasIntersection(&rect, &camera->viewport))
        GraphicsDrawRect(rect, color, filled);
}

void CameraDrawPoint(Vec2 pos, size_t radius)
{
    CameraTransformPoint(&pos);
    SDL_Rect rect = {pos.x - radius * camera->scale,
                     pos.y - radius * camera->scale,
                     radius * 2 * camera->scale,
                     radius * 2 * camera->scale};
    if (SDL_HasIntersection(&rect, &camera->viewport))
        GraphicsDraw(DrawableCreate((SDL_Rect){0, 0, 2000, 2000}, rect, SS_RedCircle));
}

void CameraDrawLine(int x1, int y1, int x2, int y2, SDL_Color color)
{
    Vec2 point1 = Vec2Create((float)x1, (float)y1);
    Vec2 point2 = Vec2Create((float)x2, (float)y2);
    CameraTransformPoint(&point1);
    CameraTransformPoint(&point2);

    int x1_t = point1.x;
    int y1_t = point1.y;
    int x2_t = point2.x;
    int y2_t = point2.y;

    if (SDL_IntersectRectAndLine(&camera->viewport, &x1_t, &y1_t, &x2_t, &y2_t))
        GraphicsDrawLine(x1_t, y1_t, x2_t, y2_t, color);
}

void CameraTransformRect(SDL_Rect *rect)
{
    rect->x -= camera->position.x;
    rect->y -= camera->position.y;
    Vec2 mid = CameraMiddle(camera);
    rect->x -= mid.x;
    rect->y -= mid.y;
    rect->x *= camera->scale;
    rect->y *= camera->scale;
    rect->w *= camera->scale;
    rect->h *= camera->scale;
    rect->x += mid.x;
    rect->y += mid.y;
}

void CameraTransformPoint(Vec2 *point)
{
    point->x -= camera->position.x;
    point->y -= camera->position.y;
    Vec2 mid = CameraMiddle(camera);
    point->x -= mid.x;
    point->y -= mid.y;
    point->x *= camera->scale;
    point->y *= camera->scale;
    point->x += mid.x;
    point->y += mid.y;
}

Vec2 CameraMiddle()
{
    SDL_Rect *vp = &camera->viewport;
    return (Vec2Create)((float)vp->x + (float)vp->w / 2.0f,
                        (float)vp->y + (float)vp->h / 2.0f);
}

void CameraAddRotation(float ammount)
{
    camera->centerRotation += ammount;
}

void CameraSetFollow(Vec2 *follow)
{
    if (camera->follow == follow)
        return;
    camera->follow = follow;
    camera->newTarget = SDL_TRUE;
}

void CameraSetFollowSnap(Vec2 *follow)
{
    if (camera->follow == follow)
        return;
    camera->follow = follow;
    camera->newTarget = SDL_FALSE;
}

void CameraSetViewPort(SDL_Rect rect)
{
    camera->viewport = rect;
}

Vec2 CameraGetPos()
{
    return Vec2MulL(camera->position, camera->scale);
}

void CameraSetScale(float scale)
{
    camera->scale = scale;
}
