#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <glib.h>
#include "cJSON.h"
int fwidth, fheight;
typedef struct direction {
    int left, right;
    int up, down;
} DIR;

typedef struct animTime {
    int lastAnimationTime;
    int lastTime;
}aTime;

typedef struct items {
    SDL_Texture* text;
    SDL_Texture* weapon;
    SDL_Texture* weaponLeft;
    SDL_Rect currPosition;
    SDL_Rect sourcePostion;
    int rWidth;
    int state;
    char type;
    char text_location[250];
    void (*f)(int,...);
} ITEMS;

typedef struct mySound {
    Mix_Music** currentMusic;
    Mix_Music* music[100];
    Mix_Chunk* soundEffects[100];
    int poz;
    int pozEffects;

} MY_SOUND;

typedef struct displayText {
    GHashTable* letters;
    int width[256];
    int saHeight, saWidth;
} D_TEXT;


typedef struct inventory {
    SDL_Texture* ALL[100];
    ITEMS back[24];
    ITEMS hotbar[11];
    SDL_Rect hotbar_sizes[11];
    ITEMS mainWeapon;

} INVENTORY;

typedef struct life {
    SDL_Texture* alula;
    SDL_Texture* hearts;
    SDL_Texture* snapShot;
    SDL_FRect dest;
    SDL_Rect src;
    SDL_Rect destH;
    SDL_Rect srcH[4];
    float life_value;
    float max_life;
    int txtWidth, txtHeight;
} LIFE;


typedef struct player {
    INVENTORY inv;
    LIFE life;
    SDL_Texture* image;
    SDL_Rect playerPoz;
    SDL_Rect sourceSize;
    aTime time;
    int speed;
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
void setSizeF(SDL_FRect* object, float x, float y, float w, float h);
void resizeF(SDL_FRect* object, double ratioW, double radioH);
#endif
