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
void renderObjsInView(OBJECTS obj,SDL_Renderer *rendererTarget,SDL_Rect camera) {
    for(int i=0;i<obj.total;i++) {
        if(SDL_HasIntersection(&obj.objsSize[i],&camera)) {
            SDL_Rect offsetSize = {obj.objsSize[i].x-camera.x,obj.objsSize[i].y - camera.y,obj.objsSize[i].w,obj.objsSize[i].h};
            SDL_RenderCopy(rendererTarget,obj.objs[i],NULL,&offsetSize);
        }
    }
}