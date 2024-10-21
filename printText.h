#pragma once
#include "common.h"
void printToSreen(SDL_Renderer* rd, char message[256], SDL_Texture* box, SDL_Rect boxSz, SDL_Texture* face, int screenWidth, int screenHeight, D_TEXT alphabet, MY_SOUND sound, PLAYER mainC);
void initLetters(SDL_Renderer* rd, D_TEXT* alphabet);