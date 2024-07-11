#include "inventory.h"

int width;
int height;
SDL_Renderer *renderTarget;

SDL_Texture* createSnapShot(SDL_Rect camera, OBJECTS obj, PLAYER mainC, SDL_Texture *map) {
    SDL_Texture *snapShot = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(renderTarget, snapShot);
    SDL_RenderCopy(renderTarget,map,&camera,NULL);
    SDL_Rect renderQuad = { mainC.playerPoz.x - camera.x, mainC.playerPoz.y - camera.y, mainC.playerPoz.w, mainC.playerPoz.h };
    SDL_RenderCopy(renderTarget, mainC.image, &mainC.sourceSize, &renderQuad);
    renderObjsInView(obj,renderTarget,camera);
    SDL_SetRenderTarget(renderTarget,NULL);
    return snapShot;
}

void initSlots(SDL_Rect (*slots)[3][8]) {
    slots[0][0]->x = 455;
    slots[0][0]->y = 343;
    slots[0][0]->w = 72;
    slots[0][0]->h = 77;
}


void inventory(SDL_Rect camera,OBJECTS obj,PLAYER mainC,SDL_Renderer *render,int widthCurr,int heightCurr, SDL_Texture *map) {
    
    SDL_RenderClear(renderTarget);
    width = widthCurr;
    height = heightCurr;
    renderTarget = render;
    SDL_Texture *snapShot = createSnapShot(camera,obj,mainC,map);
    SDL_Texture *inventoryMenu = LoadTexture("inventory.png",renderTarget);
    SDL_Rect inventSize = {0,0,width,height};
    SDL_RenderCopy(renderTarget,snapShot,NULL,&inventSize);
    SDL_RenderCopy(renderTarget,inventoryMenu,NULL,&inventSize);
    SDL_RenderPresent(renderTarget);

    SDL_Rect slots[3][8];
    initSlots(&slots);
    
    int isRunning = 1;

    while(isRunning) {
        SDL_Event ev;
        while(SDL_PollEvent(&ev) != 0) {
            if(ev.type == SDL_QUIT) {
                isRunning = 0;
            }
            else if (ev.type == SDL_KEYDOWN){
                if(ev.key.keysym.sym == SDLK_i) {
                    isRunning = 0;
                }
            } else if(ev.type == SDL_MOUSEBUTTONDOWN) {
                if(ev.button.button == SDL_BUTTON_LEFT) {

                    if(ev.button.x >= slots[0][0].x && ev.button.x <= slots[0][0].x + slots[0][0].w) {
                        if(ev.button.y >= slots[0][0].y && ev.button.y <= slots[0][0].y + slots[0][0].h) {
                            printf("SLOT1\n");
                        }
                    }
                }

            }

        }
    }


    SDL_DestroyTexture(snapShot);
    SDL_DestroyTexture(inventoryMenu);

}