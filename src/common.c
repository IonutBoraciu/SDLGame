#include "common.h"
int fwidth, fheight;

void initFullSreen() {
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    fwidth = dm.w;
    fheight = dm.h;
}

SDL_Texture* LoadTexture(const char filepath[100], SDL_Renderer* rendererTarget) {

    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(filepath);

    if (!surface) {
        printf("Surface broke\n");
    }
    else {
        texture = SDL_CreateTextureFromSurface(rendererTarget, surface);
        if (!texture)
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
        //fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetTextureBlendMode(dstTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, dstTexture);
    SDL_RenderCopy(renderer, srcTexture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    return dstTexture;
}


void renderObjsInView(OBJECTS obj, SDL_Renderer* rendererTarget, SDL_Rect camera) {
    for (int i = 0; i < obj.total; i++) {
        if (SDL_HasIntersection(&obj.objsSize[i], &camera)) {
            SDL_Rect offsetSize = { obj.objsSize[i].x - camera.x,obj.objsSize[i].y - camera.y,obj.objsSize[i].w,obj.objsSize[i].h };
            SDL_RenderCopy(rendererTarget, obj.objs[i], NULL, &offsetSize);
        }
    }
}

void resize_1280(SDL_Rect* object) {
    object->x *= (float)1280 / 1920;
    object->y *= (float)720 / 1080;
    object->w *= (float)1280 / 1920;
    object->h *= (float)720 / 1080;
}

void resize_1920(SDL_Rect* object) {
    object->x *= (float)1920 / 1280;
    object->y *= (float)1080 / 720;
    object->w *= (float)1920 / 1280;
    object->h *= (float)1080 / 720;
}

void resize(SDL_Rect* object, double ratioW, double radioH) {
    object->x = (object->x * ratioW);
    object->y = (object->y * radioH);
    object->w = (object->w * ratioW);
    object->h = (object->h * radioH);
}

void setSize(SDL_Rect* object, int x, int y, int w, int h) {
    object->w = w;
    object->x = x;
    object->y = y;
    object->h = h;

}

void setSizeF(SDL_FRect* object, float x, float y, float w, float h) {
    object->w = w;
    object->x = x;
    object->y = y;
    object->h = h;
}
void resizeF(SDL_FRect* object, double ratioW, double radioH) {
    object->x = (object->x * ratioW);
    object->y = (object->y * radioH);
    object->w = (object->w * ratioW);
    object->h = (object->h * radioH);
}

SDL_Texture* create_snapshot_texture(SDL_Renderer* renderer, SDL_Texture** textures, int texture_count, int width, int height) {

    SDL_Texture* renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

    SDL_SetRenderTarget(renderer, renderTarget);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < texture_count; i++) {
        SDL_RenderCopy(renderer, textures[i], NULL, NULL);
    }

    SDL_SetRenderTarget(renderer, NULL);

    return renderTarget;
}