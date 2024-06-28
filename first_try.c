#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

typedef struct {
    int x;
    int y;
    int health;
    SDL_Texture *texture;
} Entity;

App app;

Entity player;

int keys[SDL_NUM_SCANCODES]; 

SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    texture = IMG_LoadTexture(app.renderer, filename);

    return texture;
}

void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = 100;
    dest.h = 100;

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void initSDL()
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app.window)
    {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

void findEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                keys[event.key.keysym.scancode] = 1;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.scancode] = 0;
                break;
            default:
                break;
        }
    }
}

void renderHealthBar(Entity *player) {
    int maxHealth = 50;
    SDL_Rect healthBarBackground;
    healthBarBackground.x = player->x + 20;
    healthBarBackground.y = player->y - 12;
    healthBarBackground.w = maxHealth;
    healthBarBackground.h = 10;

    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(app.renderer, &healthBarBackground);

    SDL_Rect healthBar;
    healthBar.x = player->x + 20;
    healthBar.y = player->y - 12;
    healthBar.w = player->health;
    healthBar.h = 10;

    SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(app.renderer, &healthBar);
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(app.renderer, &healthBarBackground);
}

void treatEvents() {

    if (keys[SDL_SCANCODE_D]) {
            player.x += 5;
    }
    if (keys[SDL_SCANCODE_A]) {
        player.x -= 5;
    }
    if (keys[SDL_SCANCODE_W]) {
        player.y -= 5;
    }
    if (keys[SDL_SCANCODE_S]) {
        player.y += 5;
    }
    if(keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_C]) {
        exit(0);
    }
}



int main(int argc, char *argv[])
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));

    initSDL();

    player.x = 0;
    player.y = 0;
    player.health = 50;
    player.texture = loadTexture("first.png");
    
   // atexit(cleanup);

    while (1)
    {
        prepareScene();

        findEvents();
        treatEvents();
        blit(player.texture, player.x, player.y);
        renderHealthBar(&player);
        presentScene();

        SDL_Delay(16);
    }

    return 0;
}
