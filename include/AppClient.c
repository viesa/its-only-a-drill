#include "core/AppClient.h"

#include "core/Entity.h"
#include "Items.h"
#include "Player.h"

struct AppClient
{
    SDL_bool *running;

    Graphics *gfx;
    Audio *audio;
    Font *font;
    Gui *gui;
    Camera *camera;
    Clock *clock;
    Input *input;
    Menu *menu;
    Client *client;
    NetworkMgr *netMgr;

    Drawable db[3000];
    Vec2 cameraFollow;

    Sound test;
    Sound door;
    Sound foot;
    Sound gun;
    Sound notification;
    Sound sms;
    Sound knife;
    Sound bomp;

    Item item;
    Entity entities[3];

    Player player;
};

AppClient *AppClientCreate(SDL_bool *running, Clock *clock, Input *input, Client *client)
{
    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->running = running;
    app->clock = clock;
    app->gfx = GraphicsCreate();
    app->audio = AudioCreate();
    app->font = FontCreate(app->gfx);
    app->gui = GuiCreate(app->font, app->clock);
    app->camera = CameraCreate(app->gfx, NULL);
    app->input = input;
    app->menu = MenuCreate(app->gfx, app->font);
    app->client = client;
    app->netMgr = NetworkMgrCreate();
    app->player = PlayerCreate();
    NetworkMgrAddClient(app->netMgr, app->client);

    int cnt = 0;

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
    app->door = SoundCreate(app->audio, SF_Door);
    app->foot = SoundCreate(app->audio, SF_Footsteps);
    app->gun = SoundCreate(app->audio, SF_Gun);

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

    CameraSetFollow(app->camera, &app->player.aimFollow);

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

    if (app->menu->currentState == MS_None)
        AppClientUpdate(app);

    if (InputIsKeyPressed(app->input, SDL_SCANCODE_ESCAPE))
        app->menu->currentState = MS_MainMenu;

    AppClientDraw(app);
    GraphicsPresentScreen(app->gfx);
}

void AppClientUpdate(AppClient *app)
{
    CameraUpdate(app->camera);
    NetworkMgrPollAll(app->netMgr);

    if (InputIsKeyDown(app->input, SDL_SCANCODE_W) ||
        InputIsKeyDown(app->input, SDL_SCANCODE_D) ||
        InputIsKeyDown(app->input, SDL_SCANCODE_S) ||
        InputIsKeyDown(app->input, SDL_SCANCODE_A))
        SoundPlay(&app->foot, -1);
    else
        SoundStop(&app->foot);

    if (InputIsKeyDown(app->input, SDL_SCANCODE_M))
        SoundPlay(&app->test, 0);
    else
        SoundStop(&app->test);

    if (InputIsKeyDown(app->input, SDL_SCANCODE_L))
        app->entities[1].Force.x += 50;
    if (InputIsKeyPressed(app->input, SDL_SCANCODE_J))
        app->entities[1].Force.x -= 50;
    if (InputIsKeyPressed(app->input, SDL_SCANCODE_I))
        app->entities[1].Force.y -= 50;
    if (InputIsKeyPressed(app->input, SDL_SCANCODE_K))
        app->entities[1].Force.y += 50;
    EntityUpdate(app->entities, 3, app->clock);

    PlayerUpdate(&app->player, app->input, app->clock, app->camera);
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

    //Menu
    MenuUpdate(app->menu, app->input);
}