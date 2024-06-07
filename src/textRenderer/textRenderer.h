#pragma once
#include "SDL3_image/SDL_image.h"

void initTextRenderer(SDL_Renderer *rend);
void renderText(char *text, int x, int y, int w);
void quitTextRenderer();
void setTextSize(int size);
