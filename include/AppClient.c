#include "core/AppClient.h"

#include "Items.h"
#include "Player.h"
#include "Map.h"
#include "MapList.h"
//#define DEGBUG
struct AppClient
{
    SDL_bool *running;
    State state;

    Graphics *gfx;
    Audio *audio;
    Font *font;
    Gui *gui;
    Camera *camera;
    Clock *clock;
    FpsManger *FPSControls;
    Input *input;
    Menu *menu;

    //UDP stuff (client, server messages etc.)
    UDPClient *client;
    SDL_Thread *listenThread;

    //Item item[2];
    GroundListItems groundListItems;
    Entity entities[3];
    Player player;

    Map map;
    MapList mapList;
};
void ListenToServer(void *args)
{
    UDPClient *client = (UDPClient *)args;
    SDL_mutex *m;
    while (client->isActive)
    {
        SDL_Delay(10);
        SDL_LockMutex(m);
        if (client->hasPacket)
            continue;
        UDPClientListen(client, UDPCLIENTMSGMAXLEN);
        SDL_UnlockMutex(m);
    }
}
AppClient *AppClientCreate(SDL_bool *running, Clock *clock, Input *input, UDPClient *client, FpsManger *FPSControls)
{
    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->running = running;
    app->state = StateCreate();
    app->clock = clock;
    app->FPSControls = FPSControls;
    app->gfx = GraphicsCreate();
    app->audio = AudioCreate();
    app->font = FontCreate(app->gfx);
    app->gui = GuiCreate(app->font, app->clock);
    app->camera = CameraCreate(app->gfx, NULL);
    app->input = input;
    app->menu = MenuCreate(app->gfx, app->font, &app->state);
    app->player = PlayerCreate();

    app->client = client;
#ifdef DEGBUG
    app->listenThread = SDL_CreateThread(ListenToServer, "Server Listen Thread", (void *)app->client);
#endif
    app->entities[0] = EntityCreate((Vec2){50, 50}, EntityWoman, 0);
    app->entities[0].Force.x = 500;
    app->entities[0].Force.y = 800;
    app->entities[1] = EntityCreate((Vec2){300, 0}, EntityWoman, 1);
    app->entities[2] = EntityCreate((Vec2){500, 0}, EntityWoman, 2);

    ScoreCreate(0);
    ScoreIncrement(100, 0);

    //app->item[0] = ItemCreate(ItemWoodenSword);
    //app->item[1] = ItemCreate(ItemMetalSword);
    app->groundListItems = GroundListCreate();
    app->player.entity.inventory = InventoryCreate();

    CameraSetFollow(app->camera, &app->player.aimFollow);
#ifdef DEGBUG
    if (UDPClientSend(app->client, "hej\0", 4))
    {
        printf("Sending Message: hej\n");
        SDL_Delay(100);
        if (app->client->hasPacket)
        {
            printf("Incoming Message: %s\n", app->client->pack->data);
        }
    }
#endif
    app->state.gameState = GS_Menu;
    app->state.menuState = MS_MainMenu;

    app->map.contents = NULL;
    app->map.n = 0;
    app->mapList = MapListCreate("maps");

    return app;
}
void AppClientDestroy(AppClient *app)
{
    GraphicsDestroy(app->gfx);
    AudioDestroy(app->audio);
    CameraDestroy(app->camera);

    MenuDestroy(app->menu);
    GuiDestroy(app->gui);
    FontDestroy(app->font);

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

    switch (app->state.gameState)
    {
    case GS_Menu:
    {
        MapListUpdate(&app->mapList);
        break;
    }
    case GS_Playing:
    {
        if (InputIsKeyPressed(app->input, SDL_SCANCODE_ESCAPE))
        {
            app->state.gameState = GS_Menu;
            app->state.menuState = MS_MainMenu;
            break;
        }
        CameraUpdate(app->camera);
#ifdef DegBug
        if (InputIsKeyDown(app->input, SDL_SCANCODE_L))
            app->entities[1].Force.x += 500;
        if (InputIsKeyDown(app->input, SDL_SCANCODE_J))
            app->entities[1].Force.x -= 500;
        if (InputIsKeyDown(app->input, SDL_SCANCODE_I))
            app->entities[1].Force.y -= 500;
        if (InputIsKeyDown(app->input, SDL_SCANCODE_K))
            app->entities[1].Force.y += 500;
#endif

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_Q))
        { // if player is near to the item, then take it!
            if (app->player.entity.inventory.top < MAX_PLYER_ITEMS)
            {
                for (int tmp = 0; tmp < 2; tmp++)
                {
                    if (SDL_HasIntersection(&app->player.entity.drawable.dst, &app->groundListItems.contents[tmp].drawable.dst))
                    {
                        ItemPickup(&app->player.entity.inventory, &app->groundListItems.contents[tmp], &app->groundListItems, tmp);
                        log_info("you picked up an item. \n");
                    }
                }
            }
            else
            {
                log_info("You can't pick this item. Your item list is full! \n");
            }
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_Z))
        {
            if (app->player.entity.inventory.top > 1) // can't drop the knife
            {
                ItemDrop(&app->groundListItems, &app->player.entity.inventory, app->player.entity.position);
            }
        }

        EntityUpdate(app->entities, 4, app->clock);

        PlayerUpdate(&app->player, app->input, app->clock, app->camera);

        // SDL_PixelFormat *fmt;
        // SDL_Color *color;
        // fmt = app->gfx->format;
        // Uint8 index;
        // index = *(Uint8 *)surface->pixels;
        // color = &fmt->palette->colors[index];
        // for (int i = 0; i < ; i++)
        // {
        //     for (int j = 0; j < count; j++)
        //     {
        //         /* code */
        //     }
        //     printf("\n");
        // }
        break;
    }
    default:
        break;
    }
}

void AppClientDraw(AppClient *app)
{
    switch (app->state.gameState)
    {
    case GS_Menu:
    {
        MenuUpdate(app->menu, app->input, app->FPSControls, &app->mapList, &app->map);
        break;
    }
    case GS_Playing:
    {
        if (app->map.contents)
            for (int i = 0; i < app->map.n; i++)
                EntityDraw(app->camera, &app->map.contents[i]);

        UpdateItemDraw(&app->player.entity.inventory, &app->groundListItems, app->camera);
        EntityDraw(app->camera, &app->entities[0]);
        EntityDraw(app->camera, &app->entities[1]);
        EntityDraw(app->camera, &app->entities[2]);
        PlayerDraw(&app->player, app->camera);
        GuiUpdate(app->gui);
        if (InputIsKeyDown(app->input, SDL_SCANCODE_TAB))
        {
            InventoryDisplay(app->gfx, app->camera, &app->player.entity.inventory, app->player.entity.position);
        }

        break;
    }
    default:
        break;
    }
}