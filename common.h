#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
int fwidth, fheight;
typedef struct items {
    SDL_Texture* text;
    int state;
    char type;
    char text_location[100];
} ITEMS;

typedef struct mySound {
    Mix_Chunk* soundEffects[100];
    Mix_Music* music[100];
    int poz;
    Mix_Music** currentMusic;
} MY_SOUND;


typedef struct inventory {
    ITEMS back[24];
    ITEMS hotbar[11];
    SDL_Rect hotbar_sizes[11];
    ITEMS mainWeapon;
    SDL_Texture* ALL[100];

} INVENTORY;

typedef struct life {
    SDL_Texture* alula;
    SDL_Rect dest;
    SDL_Rect src;
    int txtWidth, txtHeight;

    SDL_Texture* hearts;
    SDL_Rect destH;
    SDL_Rect srcH[4];
    float life_value;
    SDL_Texture* snapShot;
    float max_life;
} LIFE;


typedef struct player {
    SDL_Rect playerPoz;
    SDL_Rect sourceSize;
    SDL_Texture* image;
    INVENTORY inv;
    LIFE life;
} PLAYER;

typedef struct obj {
    SDL_Texture* objs[100];
    SDL_Rect objsSize[100];
    int total;
} OBJECTS;
void setSize(SDL_Rect* object, int x, int y, int w, int h);
void resize_1280(SDL_Rect* object);
void resize_1920(SDL_Rect* object);
SDL_Texture* LoadTexture(const char filepath[100], SDL_Renderer* rendererTarget);
void renderObjsInView(OBJECTS obj, SDL_Renderer* rendererTarget, SDL_Rect camera);
SDL_Texture* DeepCopyTexture(SDL_Renderer* renderer, SDL_Texture* srcTexture);
SDL_Texture* create_snapshot_texture(SDL_Renderer* renderer, SDL_Texture** textures, int texture_count, int width, int height);
void initFullSreen();
void resize(SDL_Rect* object, double ratioW, double radioH);
#endif
