#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <stdio.h>

void selectChar(char letter);
void setTextSize(int size);

SDL_Renderer *textRenderer;
SDL_Texture  *fontAtlas;

SDL_FRect characterRect;
SDL_FRect drawRect;

const int characterSize = 32;
int drawSize = 64;

void initTextRenderer(SDL_Renderer *rend)
{
  if (!IMG_Init(IMG_INIT_PNG))
  {
    printf("ERROR INITING IMAGE\n");
    return;
  }

  textRenderer = rend;
  fontAtlas = IMG_LoadTexture(rend, "res/fonts/ProFontIIxNerdFontMono.png");
  if (fontAtlas == NULL)
  {
    printf("ERROR LOADING FONT\n");
    return;
  }
  SDL_SetTextureScaleMode(fontAtlas, SDL_SCALEMODE_NEAREST);

  characterRect.w = characterSize / 2.0;
  characterRect.h = characterSize;

  setTextSize(64);
}

void setTextSize(int size)
{
  drawSize = size;
  drawRect.w = drawSize / 2.0;
  drawRect.h = drawSize;
}


void renderText(char *text, int x, int y, int w)
{
  drawRect.x = x;
  drawRect.y = y;

  int i = 0;
  while (text[i] != '\0')
  {
    
    selectChar(text[i]);
    SDL_RenderTexture(textRenderer, fontAtlas, &characterRect, &drawRect);
    drawRect.x += drawRect.w;
    if (drawRect.x > x + w)
    {
      drawRect.x = x;
      drawRect.y += drawRect.h;
    }
    i++;
  }
}

void quitTextRenderer()
{
  SDL_DestroyTexture(fontAtlas);
  IMG_Quit();
}

//man.....
void selectChar(char letter)
{
  switch (letter) {
    case '0':
      characterRect.x = characterSize * 0;
      characterRect.y = characterSize * 1;
      break;
    case '1':
      characterRect.x = characterSize * 1;
      characterRect.y = characterSize * 1;
      break;
    case '2':
      characterRect.x = characterSize * 2;
      characterRect.y = characterSize * 1;
      break;
    case '3':
      characterRect.x = characterSize * 3;
      characterRect.y = characterSize * 1;
      break;
    case '4':
      characterRect.x = characterSize * 4;
      characterRect.y = characterSize * 1;
      break;
    case '5':
      characterRect.x = characterSize * 5;
      characterRect.y = characterSize * 1;
      break;
    case '6':
      characterRect.x = characterSize * 6;
      characterRect.y = characterSize * 1;
      break;
    case '7':
      characterRect.x = characterSize * 7;
      characterRect.y = characterSize * 1;
      break;
    case '8':
      characterRect.x = characterSize * 8;
      characterRect.y = characterSize * 1;
      break;
    case '9':
      characterRect.x = characterSize * 9;
      characterRect.y = characterSize * 1;
      break;
    case ':':
      characterRect.x = characterSize * 10;
      characterRect.y = characterSize * 1;
      break;
    case 'a':
      characterRect.x = characterSize * 1;
      characterRect.y = characterSize * 2;
      break;
    case 'b':
      characterRect.x = characterSize * 2;
      characterRect.y = characterSize * 2;
      break;
    case 'c':
      characterRect.x = characterSize * 3;
      characterRect.y = characterSize * 2;
      break;
    case 'd':
      characterRect.x = characterSize * 4;
      characterRect.y = characterSize * 2;
      break;
    case 'e':
      characterRect.x = characterSize * 5;
      characterRect.y = characterSize * 2;
      break;
    case 'f':
      characterRect.x = characterSize * 6;
      characterRect.y = characterSize * 2;
      break;
    case 'g':
      characterRect.x = characterSize * 7;
      characterRect.y = characterSize * 2;
      break;
    case 'h':
      characterRect.x = characterSize * 8;
      characterRect.y = characterSize * 2;
      break;
    case 'i':
      characterRect.x = characterSize * 9;
      characterRect.y = characterSize * 2;
      break;
    case 'j':
      characterRect.x = characterSize * 10;
      characterRect.y = characterSize * 2;
      break;
    case 'k':
      characterRect.x = characterSize * 11;
      characterRect.y = characterSize * 2;
      break;
    case 'l':
      characterRect.x = characterSize * 12;
      characterRect.y = characterSize * 2;
      break;
    case 'm':
      characterRect.x = characterSize * 13;
      characterRect.y = characterSize * 2;
      break;
    case 'n':
      characterRect.x = characterSize * 14;
      characterRect.y = characterSize * 2;
      break;
    case 'o':
      characterRect.x = characterSize * 15;
      characterRect.y = characterSize * 2;
      break;
    case 'p':
      characterRect.x = characterSize * 0;
      characterRect.y = characterSize * 3;
      break;
    case 'q':
      characterRect.x = characterSize * 1;
      characterRect.y = characterSize * 3;
      break;
    case 'r':
      characterRect.x = characterSize * 2;
      characterRect.y = characterSize * 3;
      break;
    case 's':
      characterRect.x = characterSize * 3;
      characterRect.y = characterSize * 3;
      break;
    case 't':
      characterRect.x = characterSize * 4;
      characterRect.y = characterSize * 3;
      break;
    case 'u':
      characterRect.x = characterSize * 5;
      characterRect.y = characterSize * 3;
      break;
    case 'v':
      characterRect.x = characterSize * 6;
      characterRect.y = characterSize * 3;
      break;
    case 'w':
      characterRect.x = characterSize * 7;
      characterRect.y = characterSize * 3;
      break;
    case 'x':
      characterRect.x = characterSize * 8;
      characterRect.y = characterSize * 3;
      break;
    case 'y':
      characterRect.x = characterSize * 9;
      characterRect.y = characterSize * 3;
      break;
    case 'z':
      characterRect.x = characterSize * 10;
      characterRect.y = characterSize * 3;
      break;
    case '^':
      characterRect.x = characterSize * 14;
      characterRect.y = characterSize * 3;
      break;
    case '-':
      characterRect.x = characterSize * 13;
      characterRect.y = characterSize * 0;
      break;
    case '+':
      characterRect.x = characterSize * 11;
      characterRect.y = characterSize * 0;
      break;
    default:
      characterRect.x = characterSize * 0;
      characterRect.y = characterSize * 0;
  }


  characterRect.x += characterSize / 4.0;
}
