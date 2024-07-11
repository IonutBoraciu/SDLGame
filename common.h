#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct player {
    SDL_Rect playerPoz;
    SDL_Rect sourceSize;
    SDL_Texture *image;
} PLAYER;

typedef struct obj {
    SDL_Texture *objs[100];
    SDL_Rect objsSize[100];
    int total;
} OBJECTS;

SDL_Texture *LoadTexture(const char filepath[100],SDL_Renderer *rendererTarget);
void renderObjsInView(OBJECTS obj,SDL_Renderer *rendererTarget,SDL_Rect camera);
#endif