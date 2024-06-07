#include <stdio.h>
#include "SDL3/SDL.h"

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
  SDL_Window   *window   = SDL_CreateWindow("Timer", 300, 300, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  Button startButton;
  startButton.area.w = 300;
  startButton.area.h = 50;
  startButton.area.x = 0;
  startButton.area.y = 350;

  startButton.action = &test;

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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_RenderFillRect(renderer, &startButton.area);
    SDL_RenderPresent(renderer);
  }

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
