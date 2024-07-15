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

void initSlots(SDL_Rect slots[3][8],SDL_Rect hotbar[11]) {
    slots[0][0].x = 455;
    slots[0][0].y = 343;
    slots[0][0].w = 72;
    slots[0][0].h = 77;

    float offset = 5;
    float offset2 = 0;
    float offset3 = 15;

    if(width == 1280) {
        slots[0][0].x *= (float) 1280/1920;
        slots[0][0].y *= (float) 1280/1920;
        slots[0][0].w *= (float) 1280/1920;
        slots[0][0].h *= (float) 1280/1920;
        offset *= (float)1280/1920;
        offset3 *= (float)1280/1920;
    }

    for(int j = 0; j < 3; j ++) {
        for(int i = 0; i < 8; i++) {
            if(i != 0) {
                slots[j][i].h = slots[j][i-1].h;
                slots[j][i].w = slots[j][i-1].w;
                slots[j][i].y = slots[0][0].y + offset2;
                slots[j][i].x = slots[j][i-1].x + slots[j][i-1].w + offset;
            } else {
                slots[j][i].h = slots[0][0].h;
                slots[j][i].w = slots[0][0].w;
                slots[j][i].y = slots[0][0].y + offset2;
                slots[j][i].x = slots[0][0].x;
            }
        }
        offset2 += offset3 + slots[j][0].h;
    }

    hotbar[0].x = 455;
    hotbar[0].y = 760;
    hotbar[0].w = 72;
    hotbar[0].h = 77;
    if(width == 1280) {
        hotbar[0].x *= (float) 1280/1920;
        hotbar[0].y *= (float) 1280/1920;
        hotbar[0].h *= (float) 1280/1920;
        hotbar[0].w *= (float) 1280/1920;
    }
    for(int i = 1; i < 11; i++) {
        hotbar[i].h = hotbar[i-1].h;
        hotbar[i].w = hotbar[i-1].w;
        hotbar[i].x = hotbar[i-1].x + hotbar[i-1].w + offset;
        hotbar[i].y = hotbar[i-1].y;
    }
    
}

void initBack(PLAYER mainC,SDL_Texture *snapShot,SDL_Texture *inventoryMenu,SDL_Rect inventSize,SDL_Rect slots[3][8],SDL_Rect hotbar[11]) {
    SDL_RenderClear(renderTarget);
    SDL_RenderCopy(renderTarget,snapShot,NULL,&inventSize);
    SDL_RenderCopy(renderTarget,inventoryMenu,NULL,&inventSize);
    for(int i = 0; i < 24; i++) {
        if(mainC.inv.back[i].state != 0) {
            int poz_i;
            int poz_j;
            poz_i = i/8;
            if(poz_i > 2) {
                poz_i = 2;
                poz_j = 7;
            } else {
                poz_j = i % 8;
            }
        SDL_RenderCopy(renderTarget,mainC.inv.back[i].text,NULL,&slots[poz_i][poz_j]);
        }
    }
    for(int i = 0; i < 11; i++) {
        if(mainC.inv.hotbar[i].state != 0)
            SDL_RenderCopy(renderTarget, mainC.inv.hotbar[i].text,NULL,&hotbar[i]);
    }
}

void inventory(SDL_Rect camera,OBJECTS obj,PLAYER *mainC,SDL_Renderer *render,int widthCurr,int heightCurr, SDL_Texture *map) {
    
    SDL_Rect slots[3][8];
    SDL_Rect hotbar[11];
    initSlots(slots,hotbar);

    width = widthCurr;
    height = heightCurr;
    renderTarget = render;
    SDL_Texture *snapShot = createSnapShot(camera,obj,*mainC,map);
    SDL_Texture *inventoryMenu = LoadTexture("inventory.png",renderTarget);
    SDL_Rect inventSize = {0,0,width,height};

    initBack(*mainC,snapShot,inventoryMenu,inventSize,slots,hotbar);
    SDL_RenderPresent(renderTarget);


    
    int isRunning = 1;
    int inArm = 0;
    SDL_Texture *aux = NULL;
    while(isRunning) {
        SDL_Event ev;
        if(inArm) {
            int x,y;
            SDL_GetMouseState(&x, &y);
            SDL_RenderClear(renderTarget);
            initBack(*mainC,snapShot,inventoryMenu,inventSize,slots,hotbar);
            SDL_Rect followMouse = {x,y,slots[0][0].w,slots[0][0].h};
            SDL_RenderCopy(renderTarget,aux,NULL,&followMouse);
            SDL_RenderPresent(renderTarget);
        }
        while(SDL_PollEvent(&ev) != 0) {
            if(ev.type == SDL_QUIT) {
                isRunning = 0;
            }
            else if (ev.type == SDL_KEYDOWN){
                if(ev.key.keysym.sym == SDLK_i && !inArm) {
                    isRunning = 0;
                }
            } else if(ev.type == SDL_MOUSEBUTTONDOWN) {
                if(ev.button.button == SDL_BUTTON_LEFT) {
                    for(int j = 0; j < 3; j ++) {
                        for(int i = 0; i < 8; i++) {
                            if(ev.button.x >= slots[j][i].x && ev.button.x <= slots[j][i].x + slots[j][i].w) {
                                if(ev.button.y >= slots[j][i].y && ev.button.y <= slots[j][i].y + slots[j][i].h) {
                                    if(mainC->inv.back[j*8+i].state != 0 && inArm == 0) {
                                        inArm = 1;
                                        aux = mainC->inv.back[j*8+i].text;
                                        mainC->inv.back[j*8 + i].state = 0;
                                    } else if(mainC->inv.back[j*8+i].state == 0 && inArm == 1) {
                                        inArm = 0;
                                        mainC->inv.back[j*8+i].state = 1;
                                        if (aux != mainC->inv.back[j*8+i].text) {
                                            mainC->inv.back[j*8+i].text = DeepCopyTexture(renderTarget,aux);
                                            SDL_DestroyTexture(aux);
                                        }
                                        initBack(*mainC,snapShot,inventoryMenu,inventSize,slots,hotbar);
                                        SDL_RenderPresent(renderTarget);
                                    }
                                }
                            }
                        }
                    }
                    for(int i = 0; i < 11; i++) {
                        if(ev.button.x >= hotbar[i].x && ev.button.x <= hotbar[i].x + hotbar[i].w) {
                            if(ev.button.y >= hotbar[i].y && ev.button.y <= hotbar[i].y + hotbar[i].h) {
                                if(inArm == 1) {
                                    if(mainC->inv.hotbar[i].state == 0) {
                                        inArm = 0;
                                        mainC->inv.hotbar[i].state = 1;
                                        if(aux != mainC->inv.hotbar[i].text) {
                                            mainC->inv.hotbar[i].text = DeepCopyTexture(renderTarget,aux);
                                            SDL_DestroyTexture(aux);
                                        }
                                        initBack(*mainC,snapShot,inventoryMenu,inventSize,slots,hotbar);
                                        SDL_RenderPresent(renderTarget);
                                    }
                                } else if(mainC->inv.hotbar[i].state != 0) {
                                    inArm = 1;
                                    aux = mainC->inv.hotbar[i].text;
                                    mainC->inv.hotbar[i].state = 0;
                                }
                            }
                        }
                    }
                }

            }


        }
    }


    SDL_DestroyTexture(snapShot);
    SDL_DestroyTexture(inventoryMenu);

}