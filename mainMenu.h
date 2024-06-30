#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "common.h"

int mainMenu(int *width, int *height);
void initRender(SDL_Renderer *rendererTarget);
void initWindow(SDL_Window *windowH);