#pragma once
#include "common.h"
void updateLife(PLAYER* mainC, SDL_Renderer* rd,int flag);
void initLife(PLAYER* mainC, SDL_Renderer* rd, int w, int h);
void addLife(PLAYER* mainC, float life);
void removeLife(PLAYER* mainC, float life);
void treatSmallAlula(PLAYER* mainC);