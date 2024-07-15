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

SDL_Texture* DeepCopyTexture(SDL_Renderer* renderer, SDL_Texture* srcTexture) {
    int width, height;
    SDL_QueryTexture(srcTexture, NULL, NULL, &width, &height);
    Uint32 format;
    int access;
    SDL_QueryTexture(srcTexture, &format, &access, &width, &height);
    SDL_Texture* dstTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);

    if (!dstTexture) {
        fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetTextureBlendMode(dstTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, dstTexture);
    SDL_RenderCopy(renderer, srcTexture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    return dstTexture;
}

void renderObjsInView(OBJECTS obj,SDL_Renderer *rendererTarget,SDL_Rect camera) {
    for(int i=0;i<obj.total;i++) {
        if(SDL_HasIntersection(&obj.objsSize[i],&camera)) {
            SDL_Rect offsetSize = {obj.objsSize[i].x-camera.x,obj.objsSize[i].y - camera.y,obj.objsSize[i].w,obj.objsSize[i].h};
            SDL_RenderCopy(rendererTarget,obj.objs[i],NULL,&offsetSize);
        }
    }
}