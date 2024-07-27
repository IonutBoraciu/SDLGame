#ifndef INVENTORY_H
#define INVENTORY_H
#include "common.h"

void inventory(SDL_Rect camera, OBJECTS obj, PLAYER* mainC, SDL_Renderer* render, int widthCurr, int heightCurr, SDL_Texture* map);
void addItem(PLAYER* mainC, char location[50], SDL_Renderer* render, char type);
#endif
