#include <stdio.h>
#include <string.h>
#include "SDL3/SDL.h"
#include "textRenderer/textRenderer.h"

float mouseX, mouseY;

int isMouseInside(SDL_FRect *rect);
void test();

typedef struct {
  SDL_FRect area;
  char text[21];
  void (*action)();
} Button;

int main()
{

  if ( SDL_Init(SDL_INIT_VIDEO))
  {
    printf("ERROR INITING SDL\n");
    return -1;
  }
  if (!IMG_Init(IMG_INIT_PNG))
  {
    printf("ERROR INITING IMAGE\n");
    return -1;
  }

  SDL_Window   *window   = SDL_CreateWindow("Timer", 300, 300, 0);
  if (window == NULL)
  {
    printf("ERROR LOADING WINDOW\n");
    return -1;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == NULL)
  {
    printf("ERROR LOADING RENDERER\n");
    return -1;
  }

  initTextRenderer(renderer);
  setTextSize(32);

  Button startButton;
  startButton.area.w = 300;
  startButton.area.h = 50;
  startButton.area.x = 0;
  startButton.area.y = 250;

  startButton.action = &test;
  strcpy(startButton.text, "0000");

  SDL_Event event;
  SDL_bool isRunning = SDL_TRUE;
  while(isRunning)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_EVENT_QUIT:
          isRunning = SDL_FALSE;
          break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          SDL_GetMouseState(&mouseX, &mouseY);
          if (isMouseInside(&startButton.area)) startButton.action();
          break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 100);
    SDL_RenderFillRect(renderer, &startButton.area);

    renderText(startButton.text, startButton.area.x, startButton.area.y, startButton.area.w);

    SDL_RenderPresent(renderer);
  }

  quitTextRenderer();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}

void test()
{
  printf("Hello World!\n");
}

int isMouseInside(SDL_FRect *rect)
{
  if (rect->x < mouseX && rect->x + rect->w > mouseX &&
      rect->y < mouseY && rect->y + rect->h > mouseY)
    return 1;
  return 0;
}
