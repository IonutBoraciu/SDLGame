#ifndef INVENTORY_H
#define INVENTORY_H
#include "common.h"
#include "life.h"

void inventory(SDL_Rect camera, OBJECTS obj, PLAYER* mainC, SDL_Renderer* render, int widthCurr, int heightCurr, SDL_Texture* map);
void addItem(PLAYER* mainC, char location[250], SDL_Renderer* render, char type, int frames);
#endif
