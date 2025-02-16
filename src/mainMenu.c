#include "mainMenu.h"

SDL_Renderer* renderTarget;
SDL_Window* window;
Mix_Chunk* buttonSound;
Mix_Music* menuMusic;
int resState = 1;
int width, height;
int volumeMusic = 64;
int volumeEffects = 64;

void initRender(SDL_Renderer* rendererTarget) {
    renderTarget = rendererTarget;
}

void initWindow(SDL_Window* windowH) {
    window = windowH;
}

void initSound(char path[50]) {
    buttonSound = Mix_LoadWAV(path);
}

void freeTextures(SDL_Texture* textures[10], int used) {

    for (int i = 0; i < used; i++) {
        SDL_DestroyTexture(textures[i]);
    }
}

SDL_Rect createSizeCopy(int x, int y, int widthObj, int heightObj, SDL_Texture* textureTarget) {
    SDL_Rect size;
    size.x = x;
    size.y = y;
    size.w = widthObj;
    size.h = heightObj;
    SDL_RenderCopy(renderTarget, textureTarget, NULL, &size);
    return size;
}

SDL_Rect createSize(int x, int y, int widthObj, int heightObj, SDL_Texture* textureTarget) {
    SDL_Rect size;
    size.x = x;
    size.y = y;
    size.w = widthObj;
    size.h = heightObj;
    return size;
}

void resizeElements(SDL_Rect sizes[10], SDL_Texture* textures[10], SDL_Texture* resTexts[10], SDL_Rect resSizes[10], int gap) {

    sizes[0] = createSize(0, 0, width, height, textures[0]);
    sizes[1] = createSize((width - width / 8) / 2, 8 * height / 10, width / 8, height / 12, textures[1]);
    sizes[2] = createSize((width - width / 4) / 2, (height - height / 6) / 4 + gap, width / 4, height / 6, textures[2]);
    sizes[3] = createSize((width - width / 4) / 2, (height - height / 6) / 4 + height / 6 + 2 * gap, width / 4, height / 6, textures[3]);
    SDL_SetWindowSize(window, width, height);
    SDL_Delay(20);
    resSizes[0] = createSizeCopy(0, 0, width, height, resTexts[0]);
    resSizes[1] = createSizeCopy((width - width / 8) / 2, 8 * height / 10, width / 8, height / 12, resTexts[1]);
    resSizes[2] = createSizeCopy((width - 1.5 * width / 4) / 2, (height - height / 6) / 4 + gap, width / 4, height / 8, resTexts[2]);
    resSizes[3] = createSizeCopy((width - 1.5 * width / 4) / 2, (height - height / 6) / 4 + 2 * gap + resSizes[2].h, width / 4, height / 8, resTexts[3]);
    resSizes[4] = createSizeCopy((width - 1.5 * width / 12) / 2 + resSizes[2].w - 150, (height - height / 14) / 4 + 50, width / 12, height / 15, resTexts[4]);
    resSizes[5] = createSizeCopy((width - 1.5 * width / 12) / 2 + resSizes[3].w - 150, (height - height / 14) / 4 + 110 + resSizes[2].h, width / 12, height / 15, resTexts[5]);
    resSizes[7] = createSizeCopy((width - 1.5 * width / 4.5) / 2, (height - height / 13) / 4 + 150 + resSizes[3].h + resSizes[2].h, width / 4.5, height / 13, resTexts[7]);
    resSizes[8] = createSizeCopy((width - 1.5 * width / 12) / 2 + resSizes[3].w - 150, (height - height / 14) / 4 + 150 + 2 * resSizes[2].h, width / 12, height / 15, resTexts[5]);

}

struct smollStar {
    SDL_Texture* smollText;
    SDL_Rect posSmoll;
    SDL_Rect staticSmoll;
    int smollWidth, smollHeight;
    int initial2;
    int mode;
};

void updateSmallStar(SDL_Texture* textures[10], SDL_Rect sizes[10], SDL_Rect* position, SDL_Rect* staticpoz, int smollWidth, int smollHeight, SDL_Rect position2, SDL_Rect staticpoz2, int* initial, int mode) {
    static int frames = 150;

    int currentTime = SDL_GetTicks();
    static int lastFrameTime;

    if (currentTime > lastFrameTime + frames) {
        lastFrameTime = currentTime;
        SDL_RenderClear(renderTarget);
        if (width != *initial) {
            if (width == 1280) {
                *initial = width;
                (*staticpoz).x = 33;
                (*staticpoz).y = 350;
                (*staticpoz).w = smollWidth / 6;
                (*staticpoz).h = smollHeight / 1.5;
            }
            else if (width == 1920) {
                *initial = width;
                (*staticpoz).x = width / 38;
                (*staticpoz).y = height / 3 + 1.5 * height / 10;
                (*staticpoz).w = smollWidth / 4;
                (*staticpoz).h = smollHeight;
            }
            else {
                *initial = width;
                (*staticpoz).x = width / 31;

                (*staticpoz).w = smollWidth / 2.5;
                (*staticpoz).h = smollHeight * 1.5;
                (*staticpoz).y = height / 1.8 - (*staticpoz).h / 2;
            }
        }


        if ((*position).x + smollWidth / 4 >= smollWidth) {
            (*position).x = 0;
        }
        else {
            (*position).x += smollWidth / 4;
        }
        if (mode != 3) {
            for (int i = 0; i < 4; i++) {
                SDL_RenderCopy(renderTarget, textures[i], NULL, &sizes[i]);
            }
            if (mode == 7) {
                SDL_RenderCopy(renderTarget, textures[6], NULL, &sizes[6]);
                SDL_RenderCopy(renderTarget, textures[7], NULL, &sizes[7]);
            }
        }
        if (mode == 1 || mode == 7)
            SDL_RenderCopy(renderTarget, textures[4], &position2, &staticpoz2);
        if (mode == 3) {
            for (int i = 0; i < 4; i++) {
                SDL_RenderCopy(renderTarget, textures[i], NULL, &sizes[i]);
            }
            if (resState == 1) {
                SDL_RenderCopy(renderTarget, textures[4], NULL, &sizes[4]);
                SDL_RenderCopy(renderTarget, textures[5], NULL, &sizes[5]);
                SDL_RenderCopy(renderTarget, textures[5], NULL, &sizes[8]);
            }
            else if (!resState) {
                SDL_RenderCopy(renderTarget, textures[4], NULL, &sizes[5]);
                SDL_RenderCopy(renderTarget, textures[5], NULL, &sizes[4]);
                SDL_RenderCopy(renderTarget, textures[5], NULL, &sizes[8]);
            }
            else {
                SDL_RenderCopy(renderTarget, textures[5], NULL, &sizes[5]);
                SDL_RenderCopy(renderTarget, textures[5], NULL, &sizes[4]);
                SDL_RenderCopy(renderTarget, textures[4], NULL, &sizes[8]);
            }
            SDL_RenderCopy(renderTarget, textures[6], position, staticpoz);
            SDL_RenderCopy(renderTarget, textures[7], NULL, &sizes[7]);
        }
        else {
            SDL_RenderCopy(renderTarget, textures[5], position, staticpoz);
        }
        SDL_RenderPresent(renderTarget);
    }

}

int resolutionMenu(SDL_Texture* textures[10], SDL_Rect sizes[10], int used, struct smollStar vals) {
    SDL_RenderClear(renderTarget);
    SDL_Texture* resTexts[10];
    SDL_Rect resSizes[10];
    int textSize = 0;

    // RESOLUTION BACKGROUND
    resTexts[0] = LoadTexture("assets/resolution-back.jpg", renderTarget);
    resSizes[0] = createSizeCopy(0, 0, width, height, resTexts[0]);
    textSize++;

    // BACK BUTTON
    resTexts[1] = LoadTexture("assets/back.png", renderTarget);
    resSizes[1] = createSizeCopy((width - width / 8) / 2, 8 * height / 10, width / 8, height / 12, resTexts[1]);
    textSize++;

    // 1920 SPACE
    resTexts[2] = LoadTexture("assets/1920.png", renderTarget);
    resSizes[2] = createSizeCopy((width - 1.5 * width / 4) / 2, (height - height / 6) / 4 + 50, width / 4, height / 8, resTexts[2]);
    textSize++;

    // MARK
    resTexts[4] = LoadTexture("assets/mark.jpg", renderTarget);
    resTexts[5] = LoadTexture("assets/unmark.jpg", renderTarget);
    resTexts[6] = vals.smollText;
    textSize = textSize + 2;

    resTexts[3] = LoadTexture("assets/1280.png", renderTarget);
    resSizes[3] = createSizeCopy((width - 1.5 * width / 4) / 2, (height - height / 6) / 4 + 100 + resSizes[2].h, width / 4, height / 8, resTexts[3]);
    textSize++;



    // BUTTON FOR 1920
    resSizes[4] = createSizeCopy((width - 1.5 * width / 12) / 2 + resSizes[2].w - 150, (height - height / 14) / 4 + 50, width / 12, height / 15, resTexts[4]);
    resSizes[5] = createSizeCopy((width - 1.5 * width / 12) / 2 + resSizes[3].w - 150, (height - height / 14) / 4 + 110 + resSizes[2].h, width / 12, height / 15, resTexts[4]);
    resSizes[8] = createSizeCopy((width - 1.5 * width / 12) / 2 + resSizes[3].w - 150, (height - height / 14) / 4 + 150 + 2 * resSizes[2].h, width / 12, height / 15, resTexts[4]);

    resTexts[7] = LoadTexture("assets/fullscreen.png", renderTarget);
    resSizes[7] = createSizeCopy((width - 1.5 * width / 4.5) / 2, (height - height / 13) / 4 + 150 + resSizes[3].h + resSizes[2].h, width / 4.5, height / 13, resTexts[7]);

    SDL_RenderCopy(renderTarget, vals.smollText, &vals.posSmoll, &vals.staticSmoll);
    SDL_RenderPresent(renderTarget);
    int onResolution = 1;
    int state = 0;

    while (onResolution) {
        SDL_Event ev;
        updateSmallStar(resTexts, resSizes, &vals.posSmoll, &vals.staticSmoll, vals.smollWidth, vals.smollHeight, vals.posSmoll, vals.staticSmoll, &vals.initial2, 3);
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    if (ev.button.x >= resSizes[1].x && ev.button.x <= resSizes[1].x + resSizes[1].w) {
                        if (ev.button.y >= resSizes[1].y && ev.button.y <= resSizes[1].y + resSizes[1].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            SDL_RenderClear(renderTarget);

                            for (int i = 0; i < used; i++) {
                                SDL_RenderCopy(renderTarget, textures[i], NULL, &sizes[i]);
                            }
                            updateSmallStar(resTexts, resSizes, &vals.posSmoll, &vals.staticSmoll, vals.smollWidth, vals.smollHeight, vals.posSmoll, vals.staticSmoll, &vals.initial2, 3);
                            SDL_RenderPresent(renderTarget);

                            freeTextures(resTexts, textSize);
                            SDL_DestroyTexture(resTexts[7]);

                            onResolution = 0;
                        }
                    }
                    if (ev.button.x >= resSizes[4].x && ev.button.x <= resSizes[4].x + resSizes[4].w) {
                        if (ev.button.y >= resSizes[4].y && ev.button.y <= resSizes[4].y + resSizes[4].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            resState = 1;
                            if (width != 1920 || height != 1080) {
                                SDL_RenderClear(renderTarget);
                                if (width == fwidth) {
                                    SDL_SetWindowFullscreen(window, 0);
                                }
                                width = 1920;
                                height = 1080;
                                resizeElements(sizes, textures, resTexts, resSizes, 50);
                                state = 7;
                            }

                        }
                    }
                    if (ev.button.x >= resSizes[5].x && ev.button.x <= resSizes[5].x + resSizes[5].w) {
                        if (ev.button.y >= resSizes[5].y && ev.button.y <= resSizes[5].y + resSizes[5].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            resState = 0;
                            if (width != 1280 || height != 720) {
                                SDL_RenderClear(renderTarget);
                                if (width == fwidth) {
                                    SDL_SetWindowFullscreen(window, 0);
                                }
                                width = 1280;
                                height = 720;
                                resizeElements(sizes, textures, resTexts, resSizes, 50);
                                state = 7;
                            }
                        }
                    }
                    if (ev.button.x >= resSizes[8].x && ev.button.x <= resSizes[8].x + resSizes[8].w) {
                        if (ev.button.y >= resSizes[8].y && ev.button.y <= resSizes[8].y + resSizes[8].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            resState = 2;
                            if (width != fwidth || height != fheight) {
                                SDL_RenderClear(renderTarget);
                                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                                width = fwidth;
                                height = fheight;
                                resizeElements(sizes, textures, resTexts, resSizes, 50);
                                state = 7;
                            }
                        }
                    }
                }
            }
            else if (ev.type == SDL_QUIT) {
                freeTextures(resTexts, textSize);
                SDL_DestroyTexture(resTexts[7]);
                return -1;
            }
        }
    }
    return state;

}

void displayNumber(SDL_Rect sizeNumber, SOUND* vals, int poz) {
    if (vals->volume[poz] != 10) {
        SDL_RenderCopy(renderTarget, *vals->soundText[3], &vals->form_src[poz], &sizeNumber);
    }
    else {
        (*vals).form_src[poz].x = vals->textureWidth / 10;
        SDL_RenderCopy(renderTarget, *vals->soundText[3], &vals->form_src[poz], &sizeNumber);

        (*vals).form_src[poz].x = 0;
        sizeNumber.x += sizeNumber.w + 10;
        SDL_RenderCopy(renderTarget, *vals->soundText[3], &vals->form_src[poz], &sizeNumber);
        sizeNumber.x -= sizeNumber.w + 10;
    }
}

void updateVolumeBar(SDL_Rect backButton, SDL_Texture* backText, int mode, SOUND* vals) {
    SDL_RenderClear(renderTarget);
    SDL_RenderCopy(renderTarget, *vals->soundText[0], NULL, &(*vals->soundSize[0]));
    if (vals->volume[0] != 10 && !mode) {
        vals->form_src[0].x = vals->volume[0] * (vals->textureWidth / 10);
        
    }
    if (vals->volume[1] != 10 && mode) {
        vals->form_src[1].x = vals->volume[1] * (vals->textureWidth / 10);
    }
    SDL_SetRenderDrawColor(renderTarget, 109, 12, 128, 255);
    SDL_Rect colorSize;
    colorSize = *vals->soundSize[1];
    colorSize.x += vals->offset;
    colorSize.w = vals->config[0].x - colorSize.x + vals->config[0].w;
    SDL_RenderFillRect(renderTarget, &colorSize);

    colorSize = *vals->soundSize[8];
    colorSize.x += vals->offset;
    colorSize.w = vals->config[1].x - colorSize.x + vals->config[1].w;
    SDL_RenderFillRect(renderTarget, &colorSize);

    for (int i = 1; i < vals->numberT; i++) {
        if (i == 4) {
            if (vals->volume[0]) {
                SDL_RenderCopy(renderTarget, *vals->soundText[4], NULL, &(*vals->soundSize[4]));
            }
            else {
                SDL_RenderCopy(renderTarget, *vals->soundText[5], NULL, &(*vals->soundSize[4]));
            }
        }
        else if (i == 2) {
            SDL_RenderCopy(renderTarget, *vals->soundText[2], NULL, &(vals->config[0]));

        }
        else if (i == 3) {
            displayNumber(*vals->soundSize[3], vals, 0);
            displayNumber(*vals->soundSize[9], vals, 1);
        }
        else {
            SDL_RenderCopy(renderTarget, *vals->soundText[i], NULL, &(*vals->soundSize[i]));
        }
    }
    if (vals->volume[1])
        SDL_RenderCopy(renderTarget, *vals->soundText[4], NULL, &(*vals->soundSize[10]));
    else {
        SDL_RenderCopy(renderTarget, *vals->soundText[5], NULL, &(*vals->soundSize[10]));
    }
    SDL_RenderCopy(renderTarget, *vals->soundText[2], NULL, &(vals->config[1]));
    SDL_RenderCopy(renderTarget, backText, NULL, &backButton);
    SDL_RenderPresent(renderTarget);
    if (!mode)
        Mix_VolumeChunk(buttonSound, vals->volumeLevel);
    else
        Mix_VolumeMusic(vals->volumeLevel);
    Mix_PlayChannel(-1, buttonSound, 0);

}

void createNumbers(SDL_Texture* numbers, SDL_Rect* numbersSize, int offset, int volume, int* txtWidth, int* txtHeight, int heightOffset, SOUND* vals, int poz)
{
    SDL_QueryTexture(numbers, NULL, NULL, txtWidth, txtHeight);
    vals->form_src[poz].x = volume * (*txtWidth) / 10;
    vals->form_src[poz].y = 0;
    vals->form_src[poz].w = (*txtWidth) / 10;
    vals->form_src[poz].h = (*txtHeight);
    (*numbersSize) = createSize((width - width / 2) / 2 + (width / 2) + 50, (height - height / 12) / 4 + heightOffset + offset, width / 20, height / 12, numbers);
    if (volume != 10) {
        SDL_RenderCopy(renderTarget, numbers, &vals->form_src[poz], numbersSize);
    }
    else {
        vals->form_src[poz].x = (*txtWidth) / 10;
        SDL_RenderCopy(renderTarget, numbers, &vals->form_src[poz], numbersSize);

        vals->form_src[poz].x = 0;
        (*numbersSize).x += (*numbersSize).w + 10;
        SDL_RenderCopy(renderTarget, numbers, &vals->form_src[poz], numbersSize);
        (*numbersSize).x -= (*numbersSize).w + 10;
    }
}

void progressSound(SDL_Rect* config, int offset, int heightOffset, SDL_Rect soundSize[20], SDL_Texture* soundText[20], int flag, int poz, int* oldWidth)
{
    int offset2 = 0;
    if (poz == 1) {
        offset2 = offset;
    }
    else {
        offset2 = 1.2 * (3 * offset + soundSize[1].h + soundSize[7].h);
    }
    if ((*config).x == 0 && *oldWidth == width) {

        *config = createSize((width - width / 2) / 2 + (soundSize[poz].w - soundSize[poz].x) - offset, height / 4 + soundSize[6].h + offset2 / 1.2, width / 28, height / 18, soundText[2]);
    }
    else if (*oldWidth != width) {
        (*config).x = (*config).x * width / *oldWidth;
        (*config).y = height / 4 + heightOffset + offset2 / 1.2;
        (*config).w = width / 28;
        (*config).h = height / 18;
        *oldWidth = width;
    }
    SDL_SetRenderDrawColor(renderTarget, 109, 12, 128, 255);
    SDL_Rect colorSize;
    colorSize = soundSize[poz];
    colorSize.x += offset;
    colorSize.w = (*config).x - colorSize.x + (*config).w;
    SDL_RenderFillRect(renderTarget, &colorSize);
}

void computeNewVolume(SDL_Rect size, int poz, SOUND* vals)
{
    if (vals->volume[poz]) {
        vals->volume[poz] = 0;
        vals->config[poz].x = size.x + (*vals).offset;

    }
    else {
        vals->config[poz].x = size.x + 5.5 * size.w / 11;
        vals->volume[poz] = 5;
    }
    vals->form_src[poz].x = vals->volume[poz] * ((*vals).textureWidth / 10);
    if (vals->volume[poz] != 0) {
        vals->volumeLevel = vals->volume[poz] * 12 + 8;
    }
}

void initSoundValues(SDL_Rect config, SDL_Rect configSecond, int volume, int volumeLevel, int volumeSecond, SOUND* vals) {
    vals->config[0] = config;
    vals->config[1] = configSecond;
    vals->volume[0] = volume;
    vals->volumeLevel = volumeLevel;
    vals->volume[1] = volumeSecond;
}

int soundSetting(SETT* sett) {

    SDL_RenderClear(renderTarget);
    int numberT = 0;
    SDL_Texture* soundText[20];
    SDL_Rect soundSize[20];
    static int currentWidth = 0;
    if (currentWidth == 0)
        currentWidth = width;

    int offset = 0;
    if (width == 1280) {
        offset = 30;
    }
    else if (width == 1920) {
        offset = 45;
    }
    else {
        offset = width / 45;
    }
    SOUND vals;
    vals.offset = offset;
    for (int i = 0; i < 20; i++) {
        vals.soundText[i] = &soundText[i];
        vals.soundSize[i] = &soundSize[i];
    }
    soundText[0] = LoadTexture("assets/soundBack.jpg", renderTarget);
    soundSize[0] = createSizeCopy(0, 0, width, height, soundText[0]);
    numberT++;

    soundText[6] = LoadTexture("assets/soundEffects.png", renderTarget);
    soundSize[6] = createSizeCopy((width - width / 3) / 2, (height - height / 16) / 4, width / 3, height / 16, soundText[6]);
    numberT++;

    int textureWidth, textureHeight;
    soundText[1] = LoadTexture("assets/soundBar.png", renderTarget);
    soundSize[1] = createSize((width - width / 2) / 2, (height - height / 12) / 4 + soundSize[6].h + offset, width / 2, height / 12, soundText[1]);
    numberT++;

    soundText[2] = LoadTexture("assets/smallSound.png", renderTarget);
    numberT++;

    soundText[3] = LoadTexture("assets/numbers.png", renderTarget);
    SDL_QueryTexture(soundText[3], NULL, NULL, &textureWidth, &textureHeight);
    SDL_Rect form_src;
    createNumbers(soundText[3], &soundSize[3], offset, *sett->volume[0], &textureWidth, &textureHeight, soundSize[6].h, &vals, 0);
    form_src = vals.form_src[0];
    numberT++;
    int old_val = currentWidth;
    progressSound(sett->config[0], offset, soundSize[6].h, soundSize, soundText, *sett->flag, 1, &currentWidth);
    currentWidth = old_val;

    numberT++;
    SDL_RenderCopy(renderTarget, soundText[1], NULL, &soundSize[1]);
    SDL_RenderCopy(renderTarget, soundText[2], NULL, sett->config[0]);
    SDL_RenderCopy(renderTarget, *sett->setText[1], NULL, sett->setSize[1]);

    soundText[4] = LoadTexture("assets/up.png", renderTarget);
    soundText[5] = LoadTexture("assets/mute.png", renderTarget);

    if (*sett->volume[0])
        soundSize[4] = createSizeCopy((width - width / 2) / 2 - width / 8 - 50, (height - height / 8) / 4 + soundSize[6].h + offset, width / 8, height / 8, soundText[4]);
    else
        soundSize[4] = createSizeCopy((width - width / 2) / 2 - width / 8 - 50, (height - height / 8) / 4 + soundSize[6].h + offset, width / 8, height / 8, soundText[5]);
    numberT++;

    soundText[7] = LoadTexture("assets/musicTheme.png", renderTarget);
    int y_offset = (height - height / 16) / 4 + soundSize[6].h + 2 * offset + soundSize[1].h;
    soundSize[7] = createSizeCopy((width - width / 3) / 2, y_offset, width / 3, height / 16, soundText[7]);
    numberT++;
    // sound bar 2
    soundText[8] = soundText[1];
    soundSize[8] = createSize((width - width / 2) / 2, y_offset + soundSize[7].h + offset, width / 2, height / 12, soundText[8]);
    numberT++;
    progressSound(sett->config[1], offset, soundSize[6].h, soundSize, soundText, *sett->flag, 8, &currentWidth);

    SDL_Rect form_src2;
    createNumbers(soundText[3], &soundSize[9], 3 * offset + soundSize[1].h + soundSize[7].h, *sett->volume[1], &textureWidth, &textureHeight, soundSize[6].h, &vals, 1);
    form_src2 = vals.form_src[1];
    if (*sett->volume[1])
        soundSize[10] = createSizeCopy((width - width / 2) / 2 - width / 8 - 50, (height - height / 8) / 4 + soundSize[6].h + 3 * offset + soundSize[1].h + soundSize[7].h, width / 8, height / 8, soundText[4]);
    else
        soundSize[10] = createSizeCopy((width - width / 2) / 2 - width / 8 - 50, (height - height / 8) / 4 + soundSize[6].h + 3 * offset + soundSize[1].h + soundSize[7].h, width / 8, height / 8, soundText[5]);
    SDL_RenderCopy(renderTarget, soundText[8], NULL, &soundSize[8]);
    SDL_RenderCopy(renderTarget, soundText[2], NULL, sett->config[1]);
    SDL_RenderPresent(renderTarget);

    int onSound = 1;
    vals.numberT = numberT;
    vals.textureWidth = textureWidth;
    vals.textureHeight = textureHeight;
    vals.form_src[1] = form_src2;
    vals.form_src[0] = form_src;

    while (onSound) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    if (ev.button.x >= sett->setSize[1]->x && ev.button.x <= sett->setSize[1]->x + sett->setSize[1]->w) {
                        if (ev.button.y >= sett->setSize[1]->y && ev.button.y <= sett->setSize[1]->y + sett->setSize[1]->h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            SDL_RenderClear(renderTarget);
                            for (int i = 0; i < numberT; i++) {
                                SDL_DestroyTexture(soundText[i]);
                            }

                            for (int i = 0; i < 4; i++) {
                                SDL_RenderCopy(renderTarget, *sett->setText[i], NULL, sett->setSize[i]);
                            }
                            SDL_RenderPresent(renderTarget);
                            freeTextures(soundText, 9);
                            onSound = 0;
                            return 1;
                        }
                    }
                    if (ev.button.x >= soundSize[1].x + offset && ev.button.x <= soundSize[1].x + soundSize[1].w - offset) {
                        if (ev.button.y >= soundSize[1].y && ev.button.y <= soundSize[1].y + soundSize[1].h) {
                            int clickPosition = ev.button.x - soundSize[1].x - offset;
                            int volumeStepWidth = soundSize[1].w / 12;
                            int volumeLevel = (clickPosition / volumeStepWidth) * (128 / 10);
                            int volume10 = volumeLevel / 12;
                            *sett->volume[0] = volume10;
                            if (volumeLevel != 0) {
                                volumeLevel += 8;
                            }
                            volumeEffects = volumeLevel;
                            if (ev.button.x + sett->config[0]->w <= soundSize[1].x + soundSize[1].w - offset) {
                                sett->config[0]->x = ev.button.x;
                            }
                            else {
                                sett->config[0]->x = soundSize[1].x + soundSize[1].w - offset - sett->config[0]->w;
                            }
                            initSoundValues(*sett->config[0], *sett->config[1], *sett->volume[0], volumeLevel, *sett->volume[1], &vals);
                            updateVolumeBar(*sett->setSize[1], *sett->setText[1], 0, &vals);
                        }
                    }
                    if (ev.button.x >= soundSize[4].x && ev.button.x <= soundSize[4].x + soundSize[4].w) {
                        if (ev.button.y >= soundSize[4].y && ev.button.y <= soundSize[4].y + soundSize[4].h) {
                            int volumeLevel = 0;
                            initSoundValues(*sett->config[0], *sett->config[1], *sett->volume[0], volumeLevel, *sett->volume[1], &vals);
                            computeNewVolume(soundSize[1], 0, &vals);
                            *sett->volume[0] = vals.volume[0];
                            *sett->config[0] = vals.config[0];
                            volumeLevel = vals.volumeLevel;
                            form_src = vals.form_src[0];
                            if (volumeEffects != 0) {
                                volumeEffects = 0;
                            }
                            else {
                                volumeEffects = 64;
                            }
                            updateVolumeBar(*sett->setSize[1], *sett->setText[1], 0, &vals);
                        }
                    }
                    if (ev.button.x >= soundSize[8].x + offset && ev.button.x <= soundSize[8].x + soundSize[8].w - offset) {
                        if (ev.button.y >= soundSize[8].y && ev.button.y <= soundSize[8].y + soundSize[8].h) {
                            int clickPosition = ev.button.x - soundSize[8].x - offset;
                            int volumeStepWidth = soundSize[8].w / 12;
                            int volumeLevel = (clickPosition / volumeStepWidth) * (128 / 10);
                            int volume10 = volumeLevel / 12;
                            *sett->volume[1] = volume10;

                            if (volumeLevel != 0) {
                                volumeLevel += 8;
                            }
                            volumeMusic = volumeLevel;
                            if (ev.button.x + sett->config[1]->w <= soundSize[1].x + soundSize[1].w - offset) {
                                sett->config[1]->x = ev.button.x;
                            }
                            else {
                                sett->config[1]->x = soundSize[1].x + soundSize[1].w - offset - sett->config[1]->w;
                            }
                            initSoundValues(*sett->config[0], *sett->config[1], *sett->volume[0], volumeLevel, *sett->volume[1], &vals);
                            updateVolumeBar(*sett->setSize[1], *sett->setText[1], 1, &vals);
                        }
                    }
                    if (ev.button.x >= soundSize[10].x && ev.button.x <= soundSize[10].x + soundSize[10].w) {
                        if (ev.button.y >= soundSize[10].y && ev.button.y <= soundSize[9].y + soundSize[10].h) {
                            int volumeLevel = 0;
                            initSoundValues(*sett->config[0], *sett->config[1], *sett->volume[0], volumeLevel, *sett->volume[1], &vals);
                            vals.offset -= 5;
                            computeNewVolume(soundSize[1], 1, &vals);
                            vals.offset += 5;
                            *sett->volume[1] = vals.volume[1];
                            *sett->config[1] = vals.config[1];
                            volumeLevel = vals.volumeLevel;
                            form_src2 = vals.form_src[1];
                            if (volumeMusic != 0) {
                                volumeMusic = 0;
                            }
                            else {
                                volumeMusic = 64;
                            }
                            updateVolumeBar(*sett->setSize[1], *sett->setText[1], 1, &vals);
                        }
                    }
                }
            }
            else if (ev.type == SDL_QUIT) {
                freeTextures(soundText, 9);
                return -1;
            }
        }

    }
    return 0;

}

int settingsMenu(int mode, SDL_Texture* textures[10], SDL_Rect sizes[10], int used, struct smollStar vals) {
    SDL_RenderClear(renderTarget);
    SDL_Texture* setText[10];
    SDL_Rect setSize[10];
    SETT sett;
    static SDL_Rect config = { 0,0,0,0 };
    static SDL_Rect configSecond = { 0,0,0,0 };
    static int volume = 5;
    static int volumeSecond = 5;
    static int flag = 0;
    sett.config[0] = &config;
    sett.config[1] = &configSecond;
    sett.volume[0] = &volume;
    sett.volume[1] = &volumeSecond;
    sett.flag = &flag;
    // BACKGROUND
    setText[0] = LoadTexture("assets/starSet.png", renderTarget);
    setSize[0] = createSizeCopy(0, 0, width, height, setText[0]);

    // BACK BUTTON
    setText[1] = LoadTexture("assets/back.png", renderTarget);
    setSize[1] = createSizeCopy((width - width / 8) / 2, 8 * height / 10, (width / 8), height / 12, setText[1]);

    // RESOLUTION
    setText[2] = LoadTexture("assets/resolution.png", renderTarget);
    setSize[2] = createSizeCopy((width - width / 4) / 2, (height - height / 6) / 4 + 50, width / 4, height / 6, setText[2]);

    setText[3] = LoadTexture("assets/sound.png", renderTarget);
    setSize[3] = createSizeCopy((width - width / 4) / 2, (height - height / 6) / 4 + 100 + height / 6, width / 4, height / 6, setText[3]);


    SDL_RenderCopy(renderTarget, textures[5], &vals.posSmoll, &vals.staticSmoll);
    for (int i = 0; i < 4; i++) {
        sett.setSize[i] = &setSize[i];
        sett.setText[i] = &setText[i];
    }


    SDL_RenderPresent(renderTarget);

    setText[5] = vals.smollText;

    int onSettings = 1;
    while (onSettings) {

        updateSmallStar(setText, setSize, &vals.posSmoll, &vals.staticSmoll, vals.smollWidth, vals.smollHeight, vals.posSmoll, vals.staticSmoll, &vals.initial2, 0);
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    if (ev.button.x >= setSize[1].x && ev.button.x <= setSize[1].x + setSize[1].w) {
                        if (ev.button.y >= setSize[1].y && ev.button.y <= setSize[1].y + setSize[1].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            onSettings = 0;
                            SDL_RenderClear(renderTarget);

                            for (int i = 0; i < used; i++) {
                                SDL_RenderCopy(renderTarget, textures[i], NULL, &sizes[i]);
                            }
                            SDL_RenderCopy(renderTarget, textures[5], &vals.posSmoll, &vals.staticSmoll);

                            SDL_RenderPresent(renderTarget);
                            freeTextures(setText, 4);
                        }
                    }
                    if (ev.button.x >= setSize[2].x && ev.button.x <= setSize[2].x + setSize[2].w) {
                        if (ev.button.y >= setSize[2].y && ev.button.y <= setSize[2].y + setSize[2].h) {
                            int initial_size = width;
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);
                            int check = resolutionMenu(setText, setSize, 4, vals);
                            if (width != initial_size) {
                                if (width == 1280)
                                    flag = 1;
                                else
                                    flag = 2;
                            }
                            if (check == -1) {
                                freeTextures(setText, 4);
                                return -1;
                            }
                            if (check == 7) {
                                sizes[0] = createSizeCopy(0, 0, width, height, textures[0]);
                                sizes[1] = createSizeCopy((width - width / 4) / 2, (height - height / 6) / 4 + height / 10, width / 4, height / 6, textures[1]);
                                sizes[2] = createSizeCopy((width - sizes[1].w) / 2, sizes[1].y + sizes[1].h + 25, width / 4, height / 6, textures[2]);
                                sizes[3] = createSizeCopy((width - sizes[1].w) / 2, sizes[1].y + 2 * sizes[1].h + 50, width / 4, height / 6, textures[3]);
                                sizes[6] = createSizeCopy((width - width / 2) / 2, 50, width / 2, height / 8, textures[6]);
                                sizes[7] = createSizeCopy(width - width / 3, height - height / 10, width / 3, height / 10, textures[7]);

                            }
                        }
                    }
                    if (ev.button.x >= setSize[3].x && ev.button.x <= setSize[3].x + setSize[3].w) {
                        if (ev.button.y >= setSize[3].y && ev.button.y <= setSize[3].y + setSize[3].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(150);

                            int check = soundSetting(&sett);
                            if (check == -1) {
                                freeTextures(setText, 4);
                                return -1;
                            }

                        }
                    }
                }
            }
            else if (ev.type == SDL_QUIT) {
                freeTextures(setText, 4);
                return -1;
            }
        }
    }
    return 0;
}
void initStarSky(SDL_Rect* position, SDL_Rect* staticpoz, int textureWidth, int textureHeight) {

    if (width == 1920)
        setSize(staticpoz, width - textureWidth / 12, 0, textureWidth / 12, textureHeight);
    else
        setSize(staticpoz, width - textureWidth / 14, 0, textureWidth / 14, textureHeight - 50);

    setSize(position, 0, 0, textureWidth / 12, textureHeight);
}
void updateStarSky(SDL_Texture* textures[10], SDL_Rect sizes[10], SDL_Rect* position, SDL_Rect* staticpoz, int textureWidth, int textureHeight, int* initial, SDL_Rect position2, SDL_Rect staticpoz2) {
    static int frames = 150;

    int currentTime = SDL_GetTicks();
    static int lastFrameTime;
    if (currentTime > lastFrameTime + frames) {
        lastFrameTime = currentTime;
        SDL_RenderClear(renderTarget);
        if (width != *initial) {
            if (width == 1280) {
                *initial = width;
                (*staticpoz).x = width - textureWidth / 14;
                (*staticpoz).w = textureWidth / 14;
                (*staticpoz).h = textureHeight - 50;
            }
            else {
                *initial = width;
                (*staticpoz).x = width - textureWidth / 12;
                (*staticpoz).w = textureWidth / 12;
                (*staticpoz).h = textureHeight;
            }
        }
        if ((*position).x + textureWidth / 12 >= textureWidth) {
            (*position).x = 0;

        }
        else {
            (*position).x += textureWidth / 12;
        }
        for (int i = 0; i < 4; i++) {
            SDL_RenderCopy(renderTarget, textures[i], NULL, &sizes[i]);
        }
        SDL_RenderCopy(renderTarget, textures[6], NULL, &sizes[6]);
        SDL_RenderCopy(renderTarget, textures[5], &position2, &staticpoz2);
        SDL_RenderCopy(renderTarget, textures[4], position, staticpoz);
        SDL_RenderCopy(renderTarget, textures[7], NULL, &sizes[7]);
        SDL_RenderPresent(renderTarget);
    }
}

int mainMenu(int* widthCurrent, int* heightCurrent, int mode, int* vSound, int* vEffects) {

    height = *heightCurrent;
    width = *widthCurrent;
    int onMenu = 1;
    SDL_Texture* menuTextures[10];
    SDL_Rect sizes[10];

    initSound("soundEffects/buttonClick.wav");
    Mix_VolumeChunk(buttonSound, *vEffects);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_VolumeMusic(*vSound);

    volumeMusic = *vSound;
    volumeEffects = *vEffects;

    menuMusic = Mix_LoadMUS("gameMusic/title.mp3");
    if (!menuMusic) {
        printf("Failed to load title music");
        //TO DO..free stuff or try again
        //return 0;
        //return 0;
    }
    Mix_PlayMusic(menuMusic, -1);

    menuTextures[0] = LoadTexture("assets/stars.jpg", renderTarget);
    SDL_RenderClear(renderTarget);
    sizes[0] = createSizeCopy(0, 0, width, height, menuTextures[0]);
    SDL_SetRenderTarget(renderTarget, menuTextures[0]);

    if (!mode) {
        menuTextures[6] = LoadTexture("assets/gameName.png", renderTarget);
    }
    else {
        menuTextures[6] = LoadTexture("assets/mainMenu.png", renderTarget);
    }
    sizes[6] = createSizeCopy((width - width / 2) / 2, 50, width / 2, height / 8, menuTextures[6]);

    // START BUTTON
    if (!mode) {
        menuTextures[1] = LoadTexture("assets/buttonStart.png", renderTarget);
    }
    else {
        menuTextures[1] = LoadTexture("assets/resume.png", renderTarget);
    }
    sizes[1] = createSizeCopy((width - width / 4) / 2, (height - height / 6) / 4 + height / 10, width / 4, height / 6, menuTextures[1]);

    // SETTINGS BUTTON
    menuTextures[2] = LoadTexture("assets/settings.png", renderTarget);
    sizes[2] = createSizeCopy((width - sizes[1].w) / 2, sizes[1].y + sizes[1].h + 25, width / 4, height / 6, menuTextures[2]);

    // QUIT BUTTON
    menuTextures[3] = LoadTexture("assets/quit.png", renderTarget);
    sizes[3] = createSizeCopy((width - sizes[1].w) / 2, sizes[1].y + 2 * sizes[1].h + 50, width / 4, height / 6, menuTextures[3]);





    // STARS ANIMATION
    menuTextures[4] = LoadTexture("assets/testing.png", renderTarget);
    int textureWidth, textureHeight;
    SDL_QueryTexture(menuTextures[4], NULL, NULL, &textureWidth, &textureHeight);
    SDL_Rect position;
    SDL_Rect staticpoz;
    int initial = width;
    int initial2 = width;
    initStarSky(&position, &staticpoz, textureWidth, textureHeight);

    menuTextures[5] = LoadTexture("assets/smollStar.png", renderTarget);
    int smollWidth, smollHeight;
    SDL_QueryTexture(menuTextures[5], NULL, NULL, &smollWidth, &smollHeight);
    SDL_Rect posSmoll;
    SDL_Rect staticSmoll;

    setSize(&posSmoll, 0, 0, smollWidth / 4, smollHeight);
    if (width == 1920)
        setSize(&staticSmoll, width / 38, (float)height / 3 + 1.5 * height / 10, smollWidth / 4, smollHeight);
    else if (width == 1280)
        setSize(&staticSmoll, 33, 350, smollWidth / 6, smollHeight / 1.5);
    else {
        setSize(&staticSmoll, width / 31, height / 1.8 - smollHeight * 1.5 / 2, smollWidth / 2.5, smollHeight * 1.5);
    }

    menuTextures[7] = LoadTexture("assets/author.png", renderTarget);
    sizes[7] = createSizeCopy(width - width / 3, height - height / 10, width / 3, height / 10, menuTextures[7]);

    SDL_RenderCopy(renderTarget, menuTextures[4], &position, &staticpoz);
    SDL_RenderCopy(renderTarget, menuTextures[5], &posSmoll, &staticSmoll);
    SDL_RenderPresent(renderTarget);

    while (onMenu) {

        updateSmallStar(menuTextures, sizes, &posSmoll, &staticSmoll, smollWidth, smollHeight, position, staticpoz, &initial2, 7);
        updateStarSky(menuTextures, sizes, &position, &staticpoz, textureWidth, textureHeight, &initial, posSmoll, staticSmoll);

        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                freeTextures(menuTextures, 8);
                Mix_FreeChunk(buttonSound);
                Mix_FreeMusic(menuMusic);
                return -1;
            }
            else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    // PLAY GAME
                    if (ev.button.x >= sizes[1].x && ev.button.x <= sizes[1].x + sizes[1].w) {
                        if (ev.button.y >= sizes[1].y && ev.button.y <= sizes[1].y + sizes[1].h) {
                            onMenu = 0;
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(200);
                        }

                    }

                    // EXIT GAME
                    if (ev.button.x >= sizes[3].x && ev.button.x <= sizes[3].x + sizes[3].w) {
                        if (ev.button.y >= sizes[3].y && ev.button.y <= sizes[3].y + sizes[3].h) {
                            Mix_PlayChannel(-1, buttonSound, 0);
                            SDL_Delay(500);
                            freeTextures(menuTextures, 8);
                            Mix_FreeChunk(buttonSound);
                            Mix_FreeMusic(menuMusic);
                            return -1;
                        }
                    }

                    // ENTER OPTIONS
                    if (ev.button.x >= sizes[2].x && ev.button.x <= sizes[2].x + sizes[2].w) {
                        if (ev.button.y >= sizes[2].y && ev.button.y <= sizes[2].y + sizes[2].h) {
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
                            int check = settingsMenu(0, menuTextures, sizes, 4, vals);
                            if (check == -1) {
                                freeTextures(menuTextures, 8);
                                Mix_FreeChunk(buttonSound);
                                Mix_FreeMusic(menuMusic);
                                return -1;
                            }
                        }
                    }

                }

            }
        }
    }
    *widthCurrent = width;
    *heightCurrent = height;
    *vSound = volumeMusic;

    *vEffects = volumeEffects;
    freeTextures(menuTextures, 8);
    Mix_FreeChunk(buttonSound);
    Mix_FreeMusic(menuMusic);
    SDL_SetRenderDrawColor(renderTarget, 0, 0, 0, 0);
    return 0;
}

