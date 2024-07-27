#pragma once
#include "common.h"

typedef struct gameMechanics {
	SDL_Rect hotbarSz;
	SDL_Texture* hotbar;

	SDL_Texture* map1;
	SDL_Rect map1Size;
} GAME_MECHANICS;

void updateCamera(PLAYER mainC, SDL_Rect border, SDL_Rect *camera);
void initGM(GAME_MECHANICS* gm, SDL_Rect camera, int sW, int sH, SDL_Renderer* rT, PLAYER* mainC);
void initHotbar(SDL_Rect* hotbar_sz, int startWidth, int startHeight);
void initButtons(PLAYER* mainC, SDL_Rect hotbar_sz, int width, int height);
void updateHotbar(SDL_Renderer* render, PLAYER* mainC);
