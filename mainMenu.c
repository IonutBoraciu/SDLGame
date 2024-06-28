#include "mainMenu.h"

SDL_Renderer *renderTarget;
SDL_Window *window;

void initRender(SDL_Renderer *rendererTarget) {
    renderTarget = rendererTarget;
}

void initWindow(SDL_Window *windowH) {
    window = windowH;
}

void freeTextures(SDL_Texture *textures[10],int used) {

    for(int i=0; i < used; i++) {
        SDL_DestroyTexture(textures[i]);
    }
}

SDL_Rect createSizeCopy(int x,int y,int width,int height,SDL_Texture *textureTarget) {
    SDL_Rect size;
    size.x = x;
    size.y = y;
    size.w = width;
    size.h = height;
    SDL_RenderCopy(renderTarget,textureTarget,NULL,&size);
    return size;
}
SDL_Rect createSize(int x,int y,int width,int height,SDL_Texture *textureTarget) {
    SDL_Rect size;
    size.x = x;
    size.y = y;
    size.w = width;
    size.h = height;
    //SDL_RenderCopy(renderTarget,textureTarget,NULL,&size);
    return size;
}

void resizeElements(SDL_Rect sizes[10],SDL_Texture *textures[10], SDL_Texture *resTexts[10],SDL_Rect resSizes[10],int width, int height) {

     sizes[0] = createSize(0,0,width,height,textures[0]);
    sizes[1] = createSize((width - width/8)/2,8 * height/10,width/8,height/12,textures[1]);
    sizes[2] = createSize((width - width/4)/2,(height - height/6)/4 + 50,width/4,height/6,textures[2]);
    SDL_SetWindowSize(window,width,height);
    SDL_Delay(20);
    resSizes[0] = createSizeCopy(0,0,width,height,resTexts[0]);
    resSizes[1] = createSizeCopy((width - width/8)/2,8 * height/10,width/8,height/12,resTexts[1]);
    resSizes[2] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 50,width/4,height/8,resTexts[2]);
    resSizes[3] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 100 + resSizes[2].h,width/4,height/8,resTexts[3]);
    resSizes[4] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[2].w - 150,(height - height/14)/4 + 50,width/12,height/15,resTexts[4]);
    resSizes[5] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[3].w - 150,(height - height/14)/4 + 110 + resSizes[2].h,width/12,height/15,resTexts[5]);
    SDL_RenderPresent(renderTarget);
}

void resizeElements_1280(SDL_Rect sizes[10],SDL_Texture *textures[10], SDL_Texture *resTexts[10],SDL_Rect resSizes[10],int width, int height) {
    sizes[0] = createSize(0,0,width,height,textures[0]);
    sizes[1] = createSize((width - width/8)/2,8 * height/10,width/8,height/12,textures[1]);
    sizes[2] = createSize((width - width/4)/2,(height - height/6)/4 + 50,width/4,height/6,textures[2]);
                            
    resSizes[0] = createSizeCopy(0,0,width,height,resTexts[0]);
    resSizes[1] = createSizeCopy((width - width/8)/2,8 * height/10,width/8,height/12,resTexts[1]);
    resSizes[2] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 50,width/4,height/8,resTexts[2]);
    resSizes[3] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 100 + resSizes[2].h,width/4,height/8,resTexts[3]);
    resSizes[5] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[3].w - 150,(height - height/14)/4 + 110 + resSizes[2].h,width/12,height/15,resTexts[4]);
    resSizes[4] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[2].w - 150,(height - height/14)/4 + 50,width/12,height/15,resTexts[5]);

    SDL_SetWindowSize(window,1280,720);
    SDL_RenderPresent(renderTarget);
}

int resolutionMenu(SDL_Texture *textures[10], SDL_Rect sizes[10], int used, int *width, int *height) {
    SDL_RenderClear(renderTarget);
    SDL_Texture *resTexts[10];
    SDL_Rect resSizes[10];
    int textSize = 0;

    // RESOLUTION BACKGROUND
    resTexts[0] = LoadTexture("assets/resolution-back.jpg",renderTarget);
    resSizes[0] = createSizeCopy(0,0,*width,*height,resTexts[0]);
    textSize++;

    // BACK BUTTON
    resTexts[1] = LoadTexture("assets/back.png",renderTarget);
    resSizes[1] = createSizeCopy((*width - (*width)/8)/2,8 * (*height)/10,(*width/8),(*height/12),resTexts[1]);
    textSize++;

    // 1920 SPACE
    resTexts[2] = LoadTexture("assets/1920.png",renderTarget);
    resSizes[2] = createSizeCopy(((*width) - 1.5*(*width)/4)/2,((*height) - (*height)/6)/4 + 50,(*width)/4,(*height)/8,resTexts[2]);
    textSize++;

    // MARK
    resTexts[4] = LoadTexture("assets/mark.jpg",renderTarget);
    resTexts[5] = LoadTexture("assets/unmark.jpg",renderTarget);
    textSize = textSize + 2;

    // BUTTON FOR 1920
    if(*width == 1920 && *height == 1080) {
        resSizes[4] = createSizeCopy(((*width) - 1.5*(*width)/12)/2 + resSizes[2].w - 150,((*height) - (*height)/14)/4 + 50,(*width)/12,(*height)/15,resTexts[4]);
    } else {
        resSizes[4] = createSizeCopy(((*width) - 1.5*(*width)/12)/2 + resSizes[2].w - 150,((*height) - (*height)/14)/4 + 50,(*width)/12,(*height)/15,resTexts[5]);
    }

    // 1280 SPACE
    resTexts[3] = LoadTexture("assets/1280.png",renderTarget);
    resSizes[3] = createSizeCopy(((*width) - 1.5*(*width)/4)/2,((*height) - (*height)/6)/4 + 100 + resSizes[2].h,(*width)/4,(*height)/8,resTexts[3]);
    textSize++;

    // BUTTON FOR 1280
    if(*width != 1280 || *height != 720) {
        resSizes[5] = createSizeCopy(((*width) - 1.5*(*width)/12)/2 + resSizes[3].w - 150,((*height) - (*height)/14)/4 + 110 + resSizes[2].h,(*width)/12,(*height)/15,resTexts[5]);
    } else {
        resSizes[5] = createSizeCopy(((*width) - 1.5*(*width)/12)/2 + resSizes[3].w - 150,((*height) - (*height)/14)/4 + 110 + resSizes[2].h,(*width)/12,(*height)/15,resTexts[4]);
    }

    SDL_RenderPresent(renderTarget);
    int onResolution = 1;
    int state = 0;

    while(onResolution) {
        SDL_Event ev;
        while(SDL_PollEvent(&ev)!=0) {
            if(ev.type == SDL_MOUSEBUTTONDOWN) {
                if(ev.button.button == SDL_BUTTON_LEFT) {
                    if(ev.button.x >= resSizes[1].x && ev.button.x <= resSizes[1].x + resSizes[1].w) {
                        if(ev.button.y >= resSizes[1].y && ev.button.y <= resSizes[1].y + resSizes[1].h) {
                            SDL_RenderClear(renderTarget);

                            for(int i = 0; i < used; i++) {
                                SDL_RenderCopy(renderTarget,textures[i],NULL,&sizes[i]);
                            }
                            SDL_RenderPresent(renderTarget);

                            for(int i = 0; i < textSize; i++) {
                                SDL_DestroyTexture(resTexts[i]);
                            }

                            onResolution = 0;
                        }
                    }  
                    if(ev.button.x >= resSizes[4].x && ev.button.x <= resSizes[4].x + resSizes[4].w) {
                        if(ev.button.y >= resSizes[4].y && ev.button.y <= resSizes[4].y + resSizes[4].h) {

                           
                           if(*width != 1920 || *height != 1080) {
                            SDL_RenderClear(renderTarget);
                            *width = 1920;
                            *height = 1080;

                            resizeElements(sizes,textures,resTexts,resSizes,*width,*height);
                            state = 7;
                           }

                        }
                    }
                    if(ev.button.x >= resSizes[5].x && ev.button.x <= resSizes[5].x + resSizes[5].w) {
                        if(ev.button.y >= resSizes[5].y && ev.button.y <= resSizes[5].y + resSizes[5].h) {
                            if(*width != 1280 || *height != 720) {
                            SDL_RenderClear(renderTarget);
                            *width = 1280;
                            *height = 720;

                            resizeElements_1280(sizes,textures,resTexts,resSizes,*width,*height);
                            state = 7;
                           }
                        }
                    }
                }
            } else if(ev.type == SDL_QUIT) {
                
                for(int i = 0; i < textSize; i++) {
                    SDL_DestroyTexture(resTexts[i]);
                }
                return -1;
            }
        }
    }
    return state;

}

int settingsMenu(int mode, SDL_Texture *textures[10], SDL_Rect sizes[10], int used, int *width, int *height) {
    SDL_RenderClear(renderTarget);
    SDL_Texture *setText[10];
    SDL_Rect setSize[10];
    // BACKGROUND
    setText[0] = LoadTexture("assets/starSet.png",renderTarget);
    setSize[0] = createSizeCopy(0,0,*width,*height,setText[0]);

    // BACK BUTTON
    setText[1] = LoadTexture("assets/back.png",renderTarget);
    setSize[1] = createSizeCopy((*width - (*width)/8)/2,8 * (*height)/10,(*width/8),(*height/12),setText[1]);

    // RESOLUTION
    setText[2] = LoadTexture("assets/resolution.png",renderTarget);
    setSize[2] = createSizeCopy((*width - *width/4)/2,(*height - *height/6)/4 + 50,(*width/4),(*height/6),setText[2]);
    SDL_RenderPresent(renderTarget);


    int onSettings = 1;
    while(onSettings) {

        SDL_Event ev;
        while(SDL_PollEvent(&ev)!=0) {
            if(ev.type == SDL_MOUSEBUTTONDOWN) {
                if(ev.button.button == SDL_BUTTON_LEFT) {
                    if(ev.button.x >= setSize[1].x && ev.button.x <= setSize[1].x + setSize[1].w) {
                        if(ev.button.y >= setSize[1].y && ev.button.y <= setSize[1].y + setSize[1].h) {
                            onSettings = 0;
                            SDL_RenderClear(renderTarget);

                            for(int i = 0; i < used; i++) {
                                SDL_RenderCopy(renderTarget,textures[i],NULL,&sizes[i]);
                            }
                            SDL_RenderPresent(renderTarget);
                            freeTextures(setText,3);
                            //SDL_SetWindowSize(window,1280,720);
                        }
                 
                    }
                    if(ev.button.x >= setSize[2].x && ev.button.x <= setSize[2].x + setSize[2].w) {
                        if(ev.button.y >= setSize[2].y && ev.button.y <= setSize[2].y + setSize[2].h) {
                            int check = resolutionMenu(setText,setSize,3,width,height);
                            if(check == -1) {
                                freeTextures(setText,3);
                                return -1;
                            }
                            if(check == 7) {
                                sizes[0] = createSize(0,0,*width,*height,textures[0]);
                                sizes[1] = createSize(((*width) - (*width)/4)/2,((*height) - (*height)/6)/4,(*width)/4,(*height)/6,textures[1]);
                                sizes[2] = createSize(((*width) - sizes[1].w)/2,sizes[1].y + sizes[1].h + 25,(*width)/4,(*height)/6,textures[2]);
                                sizes[3] = createSize(((*width) - sizes[1].w)/2,sizes[1].y + 2*sizes[1].h + 50,(*width)/4,(*height)/6,textures[3]);

                            }

                        }
                    }
                }
            } else if(ev.type == SDL_QUIT) {
                freeTextures(setText,3);
                return -1;
            }
        }
    }
    return 0;
}

int mainMenu(int *width, int *height) {
    int onMenu = 1;
    SDL_Texture *menuTextures[10];
    SDL_Rect sizes[10];

    // MAIN MENU BACKGROUND
    menuTextures[0] = LoadTexture("assets/stars.jpg",renderTarget);
    SDL_RenderClear(renderTarget);
    sizes[0] = createSizeCopy(0,0,*width,*height,menuTextures[0]);

    // START BUTTON
    menuTextures[1] = LoadTexture("assets/buttonStart.png",renderTarget);
    sizes[1] = createSizeCopy(((*width) - (*width)/4)/2,((*height) - (*height)/6)/4,(*width)/4,(*height)/6,menuTextures[1]);

    // SETTINGS BUTTON
    menuTextures[2] = LoadTexture("assets/settings.png",renderTarget);
    sizes[2] = createSizeCopy(((*width) - sizes[1].w)/2,sizes[1].y + sizes[1].h + 25,(*width)/4,(*height)/6,menuTextures[2]);

    // QUIT BUTTON
    menuTextures[3] = LoadTexture("assets/quit.png",renderTarget);
    sizes[3] = createSizeCopy(((*width) - sizes[1].w)/2,sizes[1].y + 2*sizes[1].h + 50,(*width)/4,(*height)/6,menuTextures[3]);

    // STARS ANIMATION
    menuTextures[4] = LoadTexture("testing.png",renderTarget);
    int textureWidth, textureHeight;
    SDL_QueryTexture(menuTextures[4],NULL,NULL,&textureWidth,&textureHeight);
    SDL_Rect position;
    SDL_Rect staticpoz;
    int initial = *width;
    staticpoz.x = *width - textureWidth/12;
    staticpoz.y = 0;
    staticpoz.w = textureWidth/12;
    staticpoz.h = textureHeight;
    position.x = 0;
    position.y = 0;
    position.w = textureWidth/12;
    position.h = textureHeight;
    SDL_RenderCopy(renderTarget,menuTextures[4],&position,&staticpoz);
    SDL_RenderPresent(renderTarget);
    int frameTime = 0;

    while(onMenu) {
        frameTime ++ ;
        if(frameTime >= 70000) {
         if(position.x + textureWidth/12 >= textureWidth) {
            position.x = 0;
            SDL_RenderClear(renderTarget);
            if(*width != initial) {
                if(*width == 1280) {
                    initial = *width;
                    staticpoz.x = *width - textureWidth/14;
                    staticpoz.w = textureWidth/14;
                    staticpoz.h = textureHeight - 50;
                } else {
                    initial = *width;
                    staticpoz.x = *width - textureWidth/12;
                    staticpoz.w = textureWidth/12;
                    staticpoz.h = textureHeight;
                }
            }

            sizes[0] = createSizeCopy(0,0,*width,*height,menuTextures[0]);
            sizes[1] = createSizeCopy(((*width) - (*width)/4)/2,((*height) - (*height)/6)/4,(*width)/4,(*height)/6,menuTextures[1]);
            sizes[2] = createSizeCopy(((*width) - sizes[1].w)/2,sizes[1].y + sizes[1].h + 25,(*width)/4,(*height)/6,menuTextures[2]);
            sizes[3] = createSizeCopy(((*width) - sizes[1].w)/2,sizes[1].y + 2*sizes[1].h + 50,(*width)/4,(*height)/6,menuTextures[3]);
            SDL_RenderCopy(renderTarget,menuTextures[4],&position,&staticpoz);
            SDL_RenderPresent(renderTarget);
        } else {
            position.x += textureWidth/12;
            SDL_RenderClear(renderTarget);

            sizes[0] = createSizeCopy(0,0,*width,*height,menuTextures[0]);
            sizes[1] = createSizeCopy(((*width) - (*width)/4)/2,((*height) - (*height)/6)/4,(*width)/4,(*height)/6,menuTextures[1]);
            sizes[2] = createSizeCopy(((*width) - sizes[1].w)/2,sizes[1].y + sizes[1].h + 25,(*width)/4,(*height)/6,menuTextures[2]);
            sizes[3] = createSizeCopy(((*width) - sizes[1].w)/2,sizes[1].y + 2*sizes[1].h + 50,(*width)/4,(*height)/6,menuTextures[3]);
            SDL_RenderCopy(renderTarget,menuTextures[4],&position,&staticpoz);
            SDL_RenderPresent(renderTarget);
        }
        frameTime = 0;
        }

        SDL_Event ev;
        while(SDL_PollEvent(&ev)!=0) {
        if(ev.type == SDL_QUIT) {
            freeTextures(menuTextures,4);
            return -1;
        } else if(ev.type == SDL_MOUSEBUTTONDOWN) {
            if(ev.button.button == SDL_BUTTON_LEFT) {
                // PLAY GAME
                if(ev.button.x >= sizes[1].x && ev.button.x  <= sizes[1].x + sizes[1].w) {
                    if(ev.button.y >= sizes[1].y && ev.button.y  <= sizes[1].y+ sizes[1].h) {
                        onMenu = 0;
                    }
                }

                // EXIT GAME
                if(ev.button.x >= sizes[3].x && ev.button.x <= sizes[3].x + sizes[3].w) {
                    if(ev.button.y >= sizes[3].y && ev.button.y <= sizes[3].y + sizes[3].h) {
                        freeTextures(menuTextures,4);
                        return -1;
                    }
                }

                // ENTER OPTIONS
                if(ev.button.x >= sizes[2].x && ev.button.x <= sizes[2].x + sizes[2].w) {
                    if(ev.button.y >= sizes[2].y && ev.button.y <= sizes[2].y + sizes[2].h) {
                        int check = settingsMenu(0,menuTextures,sizes,4,width,height);
                        if(check == -1) {
                            freeTextures(menuTextures,4);
                            return -1;
                        }
                        frameTime = 70000 - 1;
                    }
                }

            }

        }
    }
    }
    freeTextures(menuTextures,4);
    return 0;
}

