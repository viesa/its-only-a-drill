#include "core/AppClient.h"

#include "core/Entity.h"
#include "Items.h"
#include "Player.h"

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

    Player player;
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
    app->player = PlayerCreate();
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
    app->entities[0].Force.x = 500;
    app->entities[0].Force.y = 800;
    app->entities[1] = EntityCreate((Vec2){300, 0}, 100, 20, EntityWoman, 1);
    app->entities[2] = EntityCreate((Vec2){500, 0}, 100, 20, EntityWoman, 2);

    app->item = ItemCreate(ItemWoodenSword);

    CameraSetFollow(app->camera, &app->player.entity.position);

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

    if (InputGet(app->input, KEY_M))
        SoundPlay(app->test, 0);
    if (InputGet(app->input, KEY_O))
        SoundStop(app->test);
    if (InputGet(app->input, KEY_L))
        app->entities[1].Force.x = 500;
    if (InputGet(app->input, KEY_J))
        app->entities[1].Force.x = -500;
    EntityUpdate(app->entities, 3, app->clock);

    PlayerUpdate(&app->player, app->input, app->clock);
}

void AppClientDraw(AppClient *app)
{
    for (int i = 0; i < 2880; i++)
        CameraDraw(app->camera, app->db[i]);
    ItemDraw(app->camera, &app->item);
    EntityDraw(app->camera, &app->entities[0]);
    EntityDraw(app->camera, &app->entities[1]);
    EntityDraw(app->camera, &app->entities[2]);
    PlayerDraw(&app->player, app->camera);

    //GUI
    GuiUpdate(app->gui);
}