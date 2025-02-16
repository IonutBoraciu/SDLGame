#pragma once
#include "common.h"
#include "map.h"
void treatAuroraAnimation(SDL_Rect* playerPosition, int up, int down, int left, int right, int frameHeight, int frameWidth, int textureWidth, SDL_Rect* playerRect, int force, int* spX, int* spY, aTime* time, MAP* map,int speed);
void treatTurretAnimation(PLAYER* mainC, int side, int speedX, int speedY);
