#include "common.h"

SDL_Texture *LoadTexture(const char filepath[100],SDL_Renderer *rendererTarget) {

    SDL_Texture *texture = NULL;
    SDL_Surface *surface = IMG_Load(filepath);

    if(!surface) {
        printf("Surface broke\n");
    } else {
        texture = SDL_CreateTextureFromSurface(rendererTarget,surface);
        if(!texture)
            printf("Texture broke\n");
    }
    SDL_FreeSurface(surface);
    return texture;

}