#include "mainMenu.h"

SDL_Renderer *renderTarget;
SDL_Window *window;
Mix_Chunk* buttonSound;
int resState = 1;
void initRender(SDL_Renderer *rendererTarget) {
    renderTarget = rendererTarget;
}

void initWindow(SDL_Window *windowH) {
    window = windowH;
}

void initSound(char path[50]) {
    buttonSound = Mix_LoadWAV(path);
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
    sizes[3] = createSize((width - width/4)/2,(height - height/6)/4 + height/6 +100,width/4,height/6,textures[3]);
    SDL_SetWindowSize(window,width,height);
    SDL_Delay(20);
    resSizes[0] = createSizeCopy(0,0,width,height,resTexts[0]);
    resSizes[1] = createSizeCopy((width - width/8)/2,8 * height/10,width/8,height/12,resTexts[1]);
    resSizes[2] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 50,width/4,height/8,resTexts[2]);
    resSizes[3] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 100 + resSizes[2].h,width/4,height/8,resTexts[3]);
    resSizes[4] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[2].w - 150,(height - height/14)/4 + 50,width/12,height/15,resTexts[4]);
    resSizes[5] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[3].w - 150,(height - height/14)/4 + 110 + resSizes[2].h,width/12,height/15,resTexts[5]);

}

void resizeElements_1280(SDL_Rect sizes[10],SDL_Texture *textures[10], SDL_Texture *resTexts[10],SDL_Rect resSizes[10],int width, int height) {
    sizes[0] = createSize(0,0,width,height,textures[0]);
    sizes[1] = createSize((width - width/8)/2,8 * height/10,width/8,height/12,textures[1]);
    sizes[2] = createSize((width - width/4)/2,(height - height/6)/4 + 50,width/4,height/6,textures[2]);
    sizes[3] = createSize((width - width/4)/2,(height - height/6)/4 + height/6 +100,width/4,height/6,textures[3]);
    SDL_SetWindowSize(window,1280,720);
    SDL_Delay(20);                    
    resSizes[0] = createSizeCopy(0,0,width,height,resTexts[0]);
    resSizes[1] = createSizeCopy((width - width/8)/2,8 * height/10,width/8,height/12,resTexts[1]);
    resSizes[2] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 50,width/4,height/8,resTexts[2]);
    resSizes[3] = createSizeCopy((width - 1.5*width/4)/2,(height - height/6)/4 + 100 + resSizes[2].h,width/4,height/8,resTexts[3]);
    resSizes[5] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[3].w - 150,(height - height/14)/4 + 110 + resSizes[2].h,width/12,height/15,resTexts[4]);
    resSizes[4] = createSizeCopy((width - 1.5*width/12)/2 + resSizes[2].w - 150,(height - height/14)/4 + 50,width/12,height/15,resTexts[5]);

}

struct smollStar {
    SDL_Texture *smollText;
    SDL_Rect posSmoll;
    SDL_Rect staticSmoll;
    int smollWidth, smollHeight;
    int initial2;
    int mode;
};

void updateSmallStar(SDL_Texture *textures[10],SDL_Rect sizes[10] ,SDL_Rect *position, SDL_Rect *staticpoz,int *width,int *height,int smollWidth,int smollHeight,SDL_Rect position2,SDL_Rect staticpoz2,int *initial, int mode,int *frameTime){
    (*frameTime) = (*frameTime) + 1;
    if(*frameTime >= 60000) {
        SDL_RenderClear(renderTarget);
        if(*width != *initial) {
            if(*width == 1280) {
                *initial = *width;
                (*staticpoz).x = 33;
                (*staticpoz).y = 350;
                (*staticpoz).w = smollWidth/6;
                (*staticpoz).h = smollHeight/1.5;
             } else {
                *initial = *width;
                (*staticpoz).x = (*width)/38;
                (*staticpoz).y = (*height)/3 + 1.5 * (*height)/10;
                (*staticpoz).w = smollWidth/4;
                (*staticpoz).h = smollHeight;
             }
        }


        if((*position).x + smollWidth/4 >= smollWidth) {
            (*position).x = 0;
        } else {
            (*position).x += smollWidth/4;
        }
        if(mode != 3) {
            for(int i = 0; i < 4; i++) {
                SDL_RenderCopy(renderTarget,textures[i],NULL,&sizes[i]);
            }
        }
        if(mode == 1)
            SDL_RenderCopy(renderTarget,textures[4],&position2,&staticpoz2);
        if(mode == 3) {
            for(int i = 0; i < 4; i++) {
                SDL_RenderCopy(renderTarget,textures[i],NULL,&sizes[i]);
            }
            if(resState == 1) {
                SDL_RenderCopy(renderTarget,textures[4],NULL,&sizes[4]);
                SDL_RenderCopy(renderTarget,textures[5],NULL,&sizes[5]);
            } else {
                SDL_RenderCopy(renderTarget,textures[4],NULL,&sizes[5]);
                SDL_RenderCopy(renderTarget,textures[5],NULL,&sizes[4]);
            }
            SDL_RenderCopy(renderTarget,textures[6],position,staticpoz);
        } else {
        SDL_RenderCopy(renderTarget,textures[5],position,staticpoz);
        }
        SDL_RenderPresent(renderTarget);
        *frameTime = 0;
    }

}

int resolutionMenu(SDL_Texture *textures[10], SDL_Rect sizes[10], int used, int *width, int *height, struct smollStar vals) {
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
    resTexts[6] = vals.smollText;
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
    int frameTime = 0;

    SDL_RenderCopy(renderTarget,vals.smollText,&vals.posSmoll,&vals.staticSmoll);
    SDL_RenderPresent(renderTarget);
    int onResolution = 1;
    int state = 0;

    while(onResolution) {
        SDL_Event ev;
        updateSmallStar(resTexts,resSizes,&vals.posSmoll,&vals.staticSmoll,width,height,vals.smollWidth,vals.smollHeight,vals.posSmoll,vals.staticSmoll,&vals.initial2,3,&frameTime);
        while(SDL_PollEvent(&ev)!=0) {
            if(ev.type == SDL_MOUSEBUTTONDOWN) {
                if(ev.button.button == SDL_BUTTON_LEFT) {
                    if(ev.button.x >= resSizes[1].x && ev.button.x <= resSizes[1].x + resSizes[1].w) {
                        if(ev.button.y >= resSizes[1].y && ev.button.y <= resSizes[1].y + resSizes[1].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
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
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            resState = 1;
                            if(*width != 1920 || *height != 1080) {
                                SDL_RenderClear(renderTarget);
                                *width = 1920;
                                *height = 1080;
                                resizeElements(sizes,textures,resTexts,resSizes,*width,*height);
                                frameTime = 60000 -1;
                                state = 7;
                            }

                        }
                    }
                    if(ev.button.x >= resSizes[5].x && ev.button.x <= resSizes[5].x + resSizes[5].w) {
                        if(ev.button.y >= resSizes[5].y && ev.button.y <= resSizes[5].y + resSizes[5].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            resState = 0;
                            if(*width != 1280 || *height != 720) {
                            SDL_RenderClear(renderTarget);
                            *width = 1280;
                            *height = 720;
                            resizeElements_1280(sizes,textures,resTexts,resSizes,*width,*height);
                            frameTime = 60000 -1;
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

void updateVolumeBar(SDL_Rect soundSize[10],SDL_Texture *soundText[10],int volume,int offset,SDL_Rect config,int numberT,SDL_Rect form_src,int volumeLevel,int textureWidth,SDL_Rect backButton,SDL_Texture *backText) {
    SDL_RenderClear(renderTarget);
    SDL_RenderCopy(renderTarget,soundText[0],NULL,&soundSize[0]);

    if(volume != 10) {
        form_src.x = (volume)*(textureWidth/10);
    }
    SDL_SetRenderDrawColor(renderTarget, 109, 12, 128, 255);
    SDL_Rect colorSize;
    colorSize = soundSize[1];
    colorSize.x += offset;
    colorSize.w = config.x - colorSize.x + config.w;
    SDL_RenderFillRect(renderTarget,&colorSize);
                    
    for(int i=1;i < numberT;i++) {
        if (i == 4) {
            if(volume) {
                SDL_RenderCopy(renderTarget,soundText[4],NULL,&soundSize[4]);
            } else {
                SDL_RenderCopy(renderTarget,soundText[5],NULL,&soundSize[4]);
            }
        }
        else if(i == 2) {
                SDL_RenderCopy(renderTarget,soundText[2],NULL,&config);
        } else if (i == 3) {
            if(volume != 10)
                SDL_RenderCopy(renderTarget,soundText[3],&form_src,&soundSize[3]);
            else {
                form_src.x = textureWidth/10;
                SDL_RenderCopy(renderTarget,soundText[3],&form_src,&soundSize[3]);
  
                form_src.x = 0;
                soundSize[3].x += soundSize[3].w + 10;
                SDL_RenderCopy(renderTarget,soundText[3],&form_src,&soundSize[3]);
                soundSize[3].x -= soundSize[3].w + 10;
            }
            } else {
                SDL_RenderCopy(renderTarget,soundText[i],NULL,&soundSize[i]);
            }
    }
    SDL_RenderCopy(renderTarget,backText,NULL,&backButton);
    SDL_RenderPresent(renderTarget);

    Mix_VolumeChunk(buttonSound, volumeLevel);
    Mix_PlayChannel(-1, buttonSound, 0);
}

void createNumbers(SDL_Texture *numbers,SDL_Rect *form_src,SDL_Rect *numbersSize,int offset,int height,int width,int volume,int *txtWidth,int*txtHeight,int heightOffset) 
{
    SDL_QueryTexture(numbers,NULL,NULL,txtWidth,txtHeight);
    (*form_src).x = volume*(*txtWidth)/10;
    (*form_src).y = 0;
    (*form_src).w = (*txtWidth)/10;
    (*form_src).h = (*txtHeight);
    (*numbersSize) = createSize((width-width/2)/2+(width/2)+50,(height-height/12)/4+heightOffset+offset,width/20,height/12,numbers);
    if(volume != 10) {
        SDL_RenderCopy(renderTarget,numbers,form_src,numbersSize);
    } else {
        (*form_src).x = (*txtWidth)/10;
        SDL_RenderCopy(renderTarget,numbers,form_src,numbersSize);
  
        (*form_src).x = 0;
        (*numbersSize).x += (*numbersSize).w + 10;
        SDL_RenderCopy(renderTarget,numbers,form_src,numbersSize);
        (*numbersSize).x -= (*numbersSize).w + 10;
    }
}

void progressSound(SDL_Rect *config,int width,int height,int offset,int heightOffset,SDL_Rect soundSize[10],SDL_Texture *soundText[10],int flag) 
{
    if((*config).x == 0 && (*config).h != -1) {
        *config = createSize((width - width/2)/2 + (soundSize[1].w - soundSize[1].x) - offset,height/4+soundSize[6].h+offset/1.2,width/28,height/18,soundText[2]);
    } else if ((*config).h  == -1) {
        if(flag == 1)
            (*config).x = (*config).x * 1280/1920;
        else
            (*config).x = (*config).x * 1920/1280;
        (*config).y = height/4+heightOffset+offset/1.2;
        (*config).w = width/28;
        (*config).h = height/18;
    }
    SDL_SetRenderDrawColor(renderTarget, 109, 12, 128, 255);
    SDL_Rect colorSize;
    colorSize = soundSize[1];
    colorSize.x += offset;
    colorSize.w = (*config).x - colorSize.x + (*config).w;
    SDL_RenderFillRect(renderTarget,&colorSize);
}

int soundSetting(SDL_Texture *setText[10],SDL_Rect setSize[10],int *width, int *height, SDL_Rect *config, int flag,int *volume) {

    SDL_RenderClear(renderTarget);
    int numberT = 0;
    SDL_Texture *soundText[10];
    SDL_Rect soundSize[10];

    int offset = 0;
    if(*width == 1280) {
        offset = 30;
    } else {
        offset = 45;
    }

    soundText[0] = LoadTexture("soundBack.jpg",renderTarget);
    soundSize[0] = createSizeCopy(0,0,*width,*height,soundText[0]);
    numberT ++;


    soundText[6] = LoadTexture("soundEffects.png",renderTarget);
    soundSize[6] = createSizeCopy((*width-*width/3)/2,(*height-*height/16)/4,*width/3,*height/16,soundText[6]);
    numberT++;



    int textureWidth,textureHeight;
    soundText[1] = LoadTexture("soundBar.png",renderTarget);
    soundSize[1] = createSize((*width-*width/2)/2,(*height-*height/12)/4+soundSize[6].h+offset,*width/2,*height/12,soundText[1]);
    numberT ++;

    soundText[2] = LoadTexture("smallSound.png",renderTarget);
    numberT++;

    soundText[3] = LoadTexture("numbers.png",renderTarget);
    SDL_QueryTexture(soundText[3],NULL,NULL,&textureWidth,&textureHeight);
    SDL_Rect form_src;
    createNumbers(soundText[3],&form_src,&soundSize[3],offset,*height,*width,*volume,&textureWidth,&textureHeight,soundSize[6].h);
    numberT++;



    progressSound(config,*width,*height,offset,soundSize[6].h,soundSize,soundText,flag);

    numberT++;
    SDL_RenderCopy(renderTarget,soundText[1],NULL,&soundSize[1]);
    SDL_RenderCopy(renderTarget,soundText[2],NULL,config);
    SDL_RenderCopy(renderTarget,setText[1],NULL,&setSize[1]);

    
    soundText[4] = LoadTexture("up.png",renderTarget);
    soundText[5] = LoadTexture("mute.png",renderTarget);
    if(*volume)
        soundSize[4] = createSizeCopy((*width-*width/2)/2-*width/8-50,(*height-*height/8)/4+soundSize[6].h+offset,*width/8,*height/8,soundText[4]);
    else
        soundSize[4] = createSizeCopy((*width-*width/2)/2-*width/8-50,(*height-*height/8)/4+soundSize[6].h+offset,*width/8,*height/8,soundText[5]);
    numberT++;


    SDL_RenderPresent(renderTarget);

    int onSound = 1;

    while(onSound) {
        SDL_Event ev;
        while(SDL_PollEvent(&ev)!=0) {
            if(ev.type == SDL_MOUSEBUTTONDOWN) {
                if(ev.button.button == SDL_BUTTON_LEFT) {
                    if(ev.button.x >= setSize[1].x && ev.button.x <= setSize[1].x + setSize[1].w) {
                        if(ev.button.y >= setSize[1].y && ev.button.y <= setSize[1].y + setSize[1].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            SDL_RenderClear(renderTarget);
                            for(int i = 0; i < numberT; i++) {
                                    SDL_DestroyTexture(soundText[i]);
                            }

                            for(int i = 0; i < 4; i++) {
                                SDL_RenderCopy(renderTarget,setText[i],NULL,&setSize[i]);
                            }
                            SDL_RenderPresent(renderTarget);
                            onSound = 0;
                            return 1;
                        }
                    }
                    if (ev.button.x >= soundSize[1].x + offset && ev.button.x <= soundSize[1].x + soundSize[1].w - offset) {
                        if (ev.button.y >= soundSize[1].y  && ev.button.y <= soundSize[1].y + soundSize[1].h) {
                            int clickPosition = ev.button.x - soundSize[1].x -offset;
                            int volumeStepWidth = soundSize[1].w / 12;
                            int volumeLevel = (clickPosition / volumeStepWidth) * (MIX_MAX_VOLUME / 10);
                            int volume10 = volumeLevel/12;
                            *volume = volume10;
                        
                            if(volumeLevel != 0) {
                                volumeLevel += 8;
                            }
                            if(ev.button.x + (*config).w <= soundSize[1].x +soundSize[1].w - offset) {
                                (*config).x = ev.button.x;
                            }
                            else {
                                (*config).x = soundSize[1].x + soundSize[1].w - offset - (*config).w;
                            }
                            updateVolumeBar(soundSize,soundText,*volume,offset,*config,numberT,form_src,volumeLevel,textureWidth,setSize[1],setText[1]);
                        }
                    }
                    if(ev.button.x >= soundSize[4].x && ev.button.x <= soundSize[4].x + soundSize[4].w) {
                        if(ev.button.y >= soundSize[4].y && ev.button.y <= soundSize[4].y + soundSize[4].h) {
                            int volumeLevel = 0;
                    
                            if(*volume) {
                                *volume = 0;
                                (*config).x = soundSize[1].x + offset;
                        
                            } else {
                                (*config).x = soundSize[1].x + 5.5*soundSize[1].w/11;
                                *volume = 5;
                            }
                            form_src.x = (*volume)*(textureWidth/10);
                            if(*volume !=0) {
                                volumeLevel = *volume*12+8;
                            }
                            updateVolumeBar(soundSize,soundText,*volume,offset,*config,numberT,form_src,volumeLevel,textureWidth,setSize[1],setText[1]);
                        }
                    }
                }
            }
        }

    }
    return 0;

}

int settingsMenu(int mode, SDL_Texture *textures[10], SDL_Rect sizes[10], int used, int *width, int *height, struct smollStar vals) {
    SDL_RenderClear(renderTarget);
    SDL_Texture *setText[10];
    SDL_Rect setSize[10];
    int frameTime = 0;
    static SDL_Rect config = {0,0,0,0};
    static int volume = 5;
    static int flag = 0;
    // BACKGROUND
    setText[0] = LoadTexture("assets/starSet.png",renderTarget);
    setSize[0] = createSizeCopy(0,0,*width,*height,setText[0]);

    // BACK BUTTON
    setText[1] = LoadTexture("assets/back.png",renderTarget);
    setSize[1] = createSizeCopy((*width - (*width)/8)/2,8 * (*height)/10,(*width/8),(*height/12),setText[1]);

    // RESOLUTION
    setText[2] = LoadTexture("assets/resolution.png",renderTarget);
    setSize[2] = createSizeCopy((*width - *width/4)/2,(*height - *height/6)/4 + 50,(*width/4),(*height/6),setText[2]);

    setText[3] = LoadTexture("sound.png",renderTarget);
    setSize[3] = createSizeCopy((*width - *width/4)/2,(*height - *height/6)/4 + 100 + (*height/6),(*width/4),(*height/6),setText[3]);

    SDL_RenderCopy(renderTarget,textures[5],&vals.posSmoll,&vals.staticSmoll);



    SDL_RenderPresent(renderTarget);

    setText[5] = vals.smollText;

    int onSettings = 1;
    while(onSettings) {

        updateSmallStar(setText,setSize,&vals.posSmoll,&vals.staticSmoll,width,height,vals.smollWidth,vals.smollHeight,vals.posSmoll,vals.staticSmoll,&vals.initial2,0,&frameTime);
        SDL_Event ev;
        while(SDL_PollEvent(&ev)!=0) {
            if(ev.type == SDL_MOUSEBUTTONDOWN) {
                if(ev.button.button == SDL_BUTTON_LEFT) {
                    if(ev.button.x >= setSize[1].x && ev.button.x <= setSize[1].x + setSize[1].w) {
                        if(ev.button.y >= setSize[1].y && ev.button.y <= setSize[1].y + setSize[1].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            onSettings = 0;
                            SDL_RenderClear(renderTarget);

                            for(int i = 0; i < used; i++) {
                                SDL_RenderCopy(renderTarget,textures[i],NULL,&sizes[i]);
                            }
                            SDL_RenderCopy(renderTarget,textures[5],&vals.posSmoll,&vals.staticSmoll);
                            
                            SDL_RenderPresent(renderTarget);
                            freeTextures(setText,3);
                        }
                 
                    }
                    if(ev.button.x >= setSize[2].x && ev.button.x <= setSize[2].x + setSize[2].w) {
                        if(ev.button.y >= setSize[2].y && ev.button.y <= setSize[2].y + setSize[2].h) {
                            int initial_size = *width;
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            int check = resolutionMenu(setText,setSize,4,width,height,vals);
                            if(*width != initial_size) {
                                if(*width == 1280)
                                    flag = 1;
                                else 
                                    flag = 2;
                            }
                            if(check == -1) {
                                freeTextures(setText,4);
                                return -1;
                            }
                            if(check == 7) {
                                sizes[0] = createSize(0,0,*width,*height,textures[0]);
                                sizes[1] = createSize(((*width) - (*width)/4)/2,((*height) - (*height)/6)/4,(*width)/4,(*height)/6,textures[1]);
                                sizes[2] = createSize(((*width) - sizes[1].w)/2,sizes[1].y + sizes[1].h + 25,(*width)/4,(*height)/6,textures[2]);
                                sizes[3] = createSize(((*width) - sizes[1].w)/2,sizes[1].y + 2*sizes[1].h + 50,(*width)/4,(*height)/6,textures[3]);
                                config.h = -1;
                                
                            }
                            frameTime = 60000 - 1;
                        }
                    }
                    if(ev.button.x >= setSize[3].x && ev.button.x <= setSize[3].x + setSize[3].w) {
                        if(ev.button.y >= setSize[3].y && ev.button.y <= setSize[3].y + setSize[3].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            
                            int check = soundSetting(setText,setSize,width,height,&config,flag,&volume);
                            if (check != -1) {
                                frameTime = 60000 -1;
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
void initStarSky(SDL_Rect *position,SDL_Rect *staticpoz,int *width,int textureWidth,int textureHeight) {
    (*staticpoz).x = *width - textureWidth/12;
    (*staticpoz).y = 0;
    (*staticpoz).w = textureWidth/12;
    (*staticpoz).h = textureHeight;
    (*position).x = 0;
    (*position).y = 0;
    (*position).w = textureWidth/12;
    (*position).h = textureHeight;
}
void updateStarSky(SDL_Texture *textures[10],SDL_Rect sizes[10],SDL_Rect *position,SDL_Rect *staticpoz,int textureWidth,int *height,int *width,int textureHeight,int *initial,int *frameTime, SDL_Rect position2,SDL_Rect staticpoz2) {
    *frameTime = *frameTime + 1;
    if(*frameTime >= 70000) {
        SDL_RenderClear(renderTarget);
        if(*width != *initial) {
            if(*width == 1280) {
                *initial = *width;
                (*staticpoz).x = *width - textureWidth/14;
                (*staticpoz).w = textureWidth/14;
                (*staticpoz).h = textureHeight - 50;
            } else {
                *initial = *width;
                (*staticpoz).x = *width - textureWidth/12;
                (*staticpoz).w = textureWidth/12;
                (*staticpoz).h = textureHeight;
            }
        }
        if((*position).x + textureWidth/12 >= textureWidth) {
            (*position).x = 0;
            
        } else {
            (*position).x += textureWidth/12;
        }
        for(int i = 0; i < 4; i++) {
            SDL_RenderCopy(renderTarget,textures[i],NULL,&sizes[i]);
        }

        SDL_RenderCopy(renderTarget,textures[5],&position2,&staticpoz2);
        SDL_RenderCopy(renderTarget,textures[4],position,staticpoz);
        SDL_RenderPresent(renderTarget);
        *frameTime = 0;
    }
}

int mainMenu(int *width, int *height) {
    int onMenu = 1;
    SDL_Texture *menuTextures[10];
    SDL_Rect sizes[10];
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("not working\n");
    }

    initSound("ign.wav");
    Mix_VolumeChunk(buttonSound, MIX_MAX_VOLUME/2);

    Mix_Music *menuMusic = Mix_LoadMUS("title-theme.mp3");
    if(!menuMusic) {
        printf("Failed to load title music");
        //TO DO..free stuff or try again
        return 0;
    }
    Mix_PlayMusic(menuMusic, -1);

    menuTextures[0] = LoadTexture("assets/stars.jpg",renderTarget);
    SDL_RenderClear(renderTarget);
    sizes[0] = createSizeCopy(0,0,*width,*height,menuTextures[0]);
    SDL_SetRenderTarget(renderTarget, menuTextures[0]);

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
    int initial2 = *width;
    initStarSky(&position,&staticpoz,width,textureWidth,textureHeight);

    menuTextures[5] = LoadTexture("smollStar.png",renderTarget);
    int smollWidth, smollHeight;
    SDL_QueryTexture(menuTextures[5],NULL,NULL,&smollWidth,&smollHeight);
    SDL_Rect posSmoll;
    SDL_Rect staticSmoll;
    
    posSmoll.x = 0;
    posSmoll.y = 0;
    posSmoll.w = smollWidth/4;
    posSmoll.h = smollHeight;

    staticSmoll.x = (*width)/38;
    staticSmoll.y = (*height)/3 + 1.5 * (*height)/10;
    staticSmoll.w = smollWidth/4;
    staticSmoll.h = smollHeight;
    int frameTimeStar = 0;




    SDL_RenderCopy(renderTarget,menuTextures[4],&position,&staticpoz);
    SDL_RenderCopy(renderTarget,menuTextures[5],&posSmoll,&staticSmoll);
    SDL_RenderPresent(renderTarget);
    int frameTime = 0;

    while(onMenu) {

        updateSmallStar(menuTextures,sizes,&posSmoll,&staticSmoll,width,height,smollWidth,smollHeight,position,staticpoz,&initial2,1,&frameTimeStar);
        updateStarSky(menuTextures,sizes,&position,&staticpoz,textureWidth,height,width,textureHeight,&initial,&frameTime,posSmoll,staticSmoll);

        //initial = *width;
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
                        Mix_PlayChannel(-1, buttonSound, 0);
                        SDL_Delay(200);
                    }
 
                }

                // EXIT GAME
                if(ev.button.x >= sizes[3].x && ev.button.x <= sizes[3].x + sizes[3].w) {
                    if(ev.button.y >= sizes[3].y && ev.button.y <= sizes[3].y + sizes[3].h) {
                        Mix_PlayChannel(-1, buttonSound, 0);
                        SDL_Delay(500);
                        freeTextures(menuTextures,6);
                        return -1;
                    }
                }

                // ENTER OPTIONS
                if(ev.button.x >= sizes[2].x && ev.button.x <= sizes[2].x + sizes[2].w) {
                    if(ev.button.y >= sizes[2].y && ev.button.y <= sizes[2].y + sizes[2].h) {
                        Mix_PlayChannel(-1, buttonSound, 0);
                        SDL_Delay(150);
                        struct smollStar vals;
                        vals.initial2 = initial2;
                        vals.mode = 0;
                        vals.posSmoll = posSmoll;
                        vals.smollHeight = smollHeight;
                        vals.smollWidth = smollWidth;
                        vals.staticSmoll = staticSmoll;
                        vals.smollText = menuTextures[5];
                        //printf("T1 %d\n",vals.staticSmoll.h);
                        int check = settingsMenu(0,menuTextures,sizes,4,width,height,vals);
                        if(check == -1) {
                            freeTextures(menuTextures,6);
                            return -1;
                        }
                        //printf("T2 %d\n",vals.staticSmoll.h);
                        frameTime = 70000 - 1;
                        frameTimeStar = 60000 - 1;
                    }
                }

            }

        }
    }
    }
    freeTextures(menuTextures,6);
    return 0;
}

