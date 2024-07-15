#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct items {
    SDL_Texture *text;
    int state;
    char type;
} ITEMS;


typedef struct inventory {
    ITEMS back[24];
    ITEMS hotbar[11];

} INVENTORY;
 

typedef struct player {
    SDL_Rect playerPoz;
    SDL_Rect sourceSize;
    SDL_Texture *image;
    INVENTORY inv;
} PLAYER;

typedef struct obj {
    SDL_Texture *objs[100];
    SDL_Rect objsSize[100];
    int total;
} OBJECTS;

SDL_Texture *LoadTexture(const char filepath[100],SDL_Renderer *rendererTarget);
void renderObjsInView(OBJECTS obj,SDL_Renderer *rendererTarget,SDL_Rect camera);
SDL_Texture* DeepCopyTexture(SDL_Renderer* renderer, SDL_Texture* srcTexture);
#endif