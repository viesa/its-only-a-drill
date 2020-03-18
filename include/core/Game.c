#include "Game.h"

#include "../Resources.h"

struct Game
{
    Graphics *m_gfx;
    Audio *m_audio;
    Resources *m_resources;
    Camera *m_camera;
    Clock *m_clock;
    Input *m_input;
    void *m_attributes[GAttrib_Count];
};

Game *GameCreate(Clock *clock, SDL_bool *running, Input *input)
{
    Game *game_ret = (Game *)SDL_malloc(sizeof(Game));
    game_ret->m_gfx = GraphicsCreate();
    game_ret->m_audio = AudioCreate();
    game_ret->m_resources = (Resources *)SDL_malloc(sizeof(Resources));
    game_ret->m_camera = CameraCreate(game_ret->m_gfx, &game_ret->m_resources->cameraFollow);
    game_ret->m_clock = clock;
    game_ret->m_input = input;
    game_ret->m_attributes[GAttrib_Game] = game_ret;
    game_ret->m_attributes[GAttrib_Graphics] = game_ret->m_gfx;
    game_ret->m_attributes[GAttrib_Audio] = game_ret->m_audio;
    game_ret->m_attributes[GAttrib_Resources] = game_ret->m_resources;
    game_ret->m_attributes[GAttrib_Camera] = game_ret->m_camera;
    game_ret->m_attributes[GAttrib_Clock] = game_ret->m_clock;
    game_ret->m_attributes[GAttrib_Input] = game_ret->m_input;
    GameInit(game_ret->m_attributes);
    return game_ret;
}
void GameDestroy(Game *game)
{
    GameExit(game->m_attributes);
    GraphicsDestroy(game->m_gfx);
    AudioDestroy(game->m_audio);
    CameraDestroy(game->m_camera);

    SDL_free(game);
}

void GameRun(Game *game)
{
    GraphicsClearScreen(game->m_gfx);
    GameUpdateComponents(game);
    GameUpdateLogic(game->m_attributes);
    GameComposeFrame(game->m_attributes);
    GraphicsPresentScreen(game->m_gfx);
}

void GameUpdateComponents(Game *game)
{
    CameraUpdate(game->m_camera);
}

void Draw(Game *game, Drawable drawable)
{
    CameraDraw(game->m_camera, drawable);
}