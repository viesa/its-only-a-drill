#include "core/AppClient.h"

#include "core/Entity.h"
#include "Items.h"

struct AppClient
{
    Graphics *gfx;
    Audio *audio;
    Font *font;
    Gui *gui;
    Camera *camera;
    Clock *clock;
    Input *input;
    Client *client;
    NetworkMgr *netMgr;

    Drawable db[3000];
    Vec2 cameraFollow;
    Sound test;
    Item item;
    Entity entities[3];
};

AppClient *AppClientCreate(Clock *clock, SDL_bool *running, Input *input, Client *client)
{
    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->gfx = GraphicsCreate();
    app->audio = AudioCreate();
    app->font = FontCreate(app->gfx);
    app->gui = GuiCreate(app->font);
    app->camera = CameraCreate(app->gfx, NULL);
    app->clock = clock;
    app->input = input;
    app->client = client;
    app->netMgr = NetworkMgrCreate();
    NetworkMgrAddClient(app->netMgr, app->client);

    int cnt = 0;

    //Cut tiles from spritesheets
    //SS_Legacy
    SDL_Rect tileSand = {16, 112, 16, 16};
    SDL_Rect tileCowHead = {352, 1088, 16, 16};

    //SS_Characters
    SDL_Rect tileWomanDefaultGun = {0, 44, 57, 43};

    //SS_Tiles
    SDL_Rect tileWood = {1036, 74, 64, 64};

    short floorTileSize = 64;

    for (int row = 0; row < 45; row++)
    {
        for (int col = 0; col < 64; col++)
        {
            app->db[cnt] = DrawableCreate(tileWood, (SDL_Rect){(col - 32) * floorTileSize, (row - 22.5) * floorTileSize, floorTileSize, floorTileSize}, SS_Tiles);
            cnt++;
        }
    }

    app->db[2999] = DrawableCreate(tileWomanDefaultGun, (SDL_Rect){496, 344, 57, 43}, SS_Characters);

    app->test = SoundCreate(app->audio, SF_Test);

    app->cameraFollow = (Vec2){0.0f, 0.0f};

    ClientSetNet(client, "85.226.233.210", 1337);
    // ClientStart(client);
    // ClientSend(client, Test, "THIS IS A TEST", 15);

    app->entities[0] = EntityCreate((Vec2){0, 0}, 100, 20, EntityWoman, 0);
    app->entities[0].moveVec.x = 500;
    app->entities[1] = EntityCreate((Vec2){300, 0}, 100, 20, EntityWoman, 1);
    app->entities[2] = EntityCreate((Vec2){500, 0}, 100, 20, EntityWoman, 2);

    app->item = ItemCreate(ItemWoodenSword);

    CameraSetFollow(app->camera, &app->cameraFollow);

    return app;
}
void AppClientDestroy(AppClient *app)
{
    GraphicsDestroy(app->gfx);
    AudioDestroy(app->audio);
    CameraDestroy(app->camera);
    NetworkMgrDestroy(app->netMgr);

    SDL_free(app);
}

void AppClientRun(AppClient *app)
{
    GraphicsClearScreen(app->gfx);
    AppClientUpdate(app);
    AppClientDraw(app);
    GraphicsPresentScreen(app->gfx);
}

void AppClientUpdate(AppClient *app)
{
    CameraUpdate(app->camera);
    NetworkMgrPollAll(app->netMgr);

    if (InputGet(app->input, KEY_Q))
        CameraAddRotation(app->camera, -1.0f);
    if (InputGet(app->input, KEY_E))
        CameraAddRotation(app->camera, 1.0f);

    if (InputGet(app->input, KEY_A))
        app->cameraFollow.x -= 3.0f;
    if (InputGet(app->input, KEY_W))
        app->cameraFollow.y -= 3.0f;
    if (InputGet(app->input, KEY_D))
        app->cameraFollow.x += 3.0f;
    if (InputGet(app->input, KEY_S))
        app->cameraFollow.y += 3.0f;
    if (InputGet(app->input, KEY_M))
        SoundPlay(app->test, 0);
    if (InputGet(app->input, KEY_O))
        SoundStop(app->test);
    EntityUpdate(app->entities, 3, &app->entities[0], app->clock);
    EntityUpdate(app->entities, 3, &app->entities[1], app->clock);
    EntityUpdate(app->entities, 3, &app->entities[2], app->clock);
}

void AppClientDraw(AppClient *app)
{
    for (int i = 0; i < 2880; i++)
        GraphicsDraw(app->gfx, app->db[i]);
    ItemDraw(app->camera, &app->item);
    EntityDraw(app->camera, &app->entities[0]);
    EntityDraw(app->camera, &app->entities[1]);
    EntityDraw(app->camera, &app->entities[2]);
    PlayerDraw(app->camera, app->db[2999]);

    //GUI
    GuiUpdate(app->gui);
}