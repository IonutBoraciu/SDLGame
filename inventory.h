#ifndef INVENTORY_H
#define INVENTORY_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "common.h"

void inventory(SDL_Rect camera,OBJECTS obj,PLAYER mainC,SDL_Renderer *render,int widthCurr,int heightCurr, SDL_Texture *map);

#endif