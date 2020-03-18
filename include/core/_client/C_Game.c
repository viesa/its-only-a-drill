#include "C_Game.h"

#include "../../../_client/C_Resources.h"

struct C_Game
{
    Graphics *m_gfx;
    Audio *m_audio;
    C_Resources *m_resources;
    Camera *m_camera;
    Clock *m_clock;
    Input *m_input;
    Client *m_client;
    NetworkMgr *m_netMgr;
    void *m_attributes[GAttrib_Count];
};

C_Game *C_GameCreate(Clock *clock, SDL_bool *running, Input *input, Client *client)
{
    C_Game *ret = (C_Game *)SDL_malloc(sizeof(C_Game));
    ret->m_gfx = GraphicsCreate();
    ret->m_audio = AudioCreate();
    ret->m_resources = (C_Resources *)SDL_malloc(sizeof(C_Resources));
    ret->m_camera = CameraCreate(ret->m_gfx, &ret->m_resources->cameraFollow);
    ret->m_clock = clock;
    ret->m_input = input;
    ret->m_client = client;
    ret->m_netMgr = NetworkMgrCreate();
    NetworkMgrAddClient(ret->m_netMgr, ret->m_client);
    ret->m_attributes[GAttrib_Game] = ret;
    ret->m_attributes[GAttrib_Graphics] = ret->m_gfx;
    ret->m_attributes[GAttrib_Audio] = ret->m_audio;
    ret->m_attributes[GAttrib_Resources] = ret->m_resources;
    ret->m_attributes[GAttrib_Camera] = ret->m_camera;
    ret->m_attributes[GAttrib_Clock] = ret->m_clock;
    ret->m_attributes[GAttrib_Input] = ret->m_input;
    ret->m_attributes[GAttrib_Client] = ret->m_client;
    C_Init(ret->m_attributes);
    return ret;
}
void C_GameDestroy(C_Game *game)
{
    C_Exit(game->m_attributes);
    GraphicsDestroy(game->m_gfx);
    AudioDestroy(game->m_audio);
    CameraDestroy(game->m_camera);
    NetworkMgrDestroy(game->m_netMgr);

    SDL_free(game);
}

void C_GameRun(C_Game *game)
{
    GraphicsClearScreen(game->m_gfx);
    C_GameUpdateComponents(game);
    C_Logic(game->m_attributes);
    C_Graphics(game->m_attributes);
    GraphicsPresentScreen(game->m_gfx);
}

void C_GameUpdateComponents(C_Game *game)
{
    CameraUpdate(game->m_camera);
    NetworkMgrPollAll(game->m_netMgr);
}

void Draw(C_Game *game, Drawable drawable)
{
    CameraDraw(game->m_camera, drawable);
}