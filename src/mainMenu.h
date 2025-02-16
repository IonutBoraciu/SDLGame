#ifndef MAINMENU_H
#define MAINMENU_H

#include <stdio.h>
#include "common.h"

typedef struct soundValues {
    SDL_Rect* soundSize[20];
    SDL_Texture** soundText[20];
    int volume[2];
    int offset;
    SDL_Rect config[2];
    int numberT;
    int volumeLevel;
    int textureWidth;
    int textureHeight;
    SDL_Rect form_src[2];
} SOUND;

typedef struct settValues {
    SDL_Texture** setText[10];
    SDL_Rect* setSize[10];
    SDL_Rect* config[2];
    int* volume[2];
    int* flag;
} SETT;

int mainMenu(int* width, int* height, int mode, int* vSound, int* vEffects);
void initRender(SDL_Renderer* rendererTarget);
void initWindow(SDL_Window* windowH);
#endif

