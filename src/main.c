#include <stdio.h>
#include <string.h>
#include "SDL3/SDL.h"
#include "textRenderer/textRenderer.h"

const int textSize = 32;
const int screenSize = 320;
float mouseX, mouseY;
int timeLeft = 5;
char timerText[16] = " ";

typedef struct {
  SDL_FRect area;
  char text[21];
  void (*action)();
  SDL_bool isVisible;
} Button;

void safeInit(int result);
void* safeCreate(void *result);
void renderButtonText(Button *btn);
void drawButton(SDL_Renderer *renderer, Button *btn);
int isMouseInside(SDL_FRect *rect);
void updateTimer();
void test();

int main()
{

  safeInit(SDL_Init(SDL_INIT_VIDEO));
  safeInit(!IMG_Init(IMG_INIT_PNG));

  SDL_Window   *window   = safeCreate(SDL_CreateWindow("Timer", screenSize, screenSize, 0));
  SDL_Renderer *renderer = safeCreate(SDL_CreateRenderer(window, NULL));
  SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

  initTextRenderer(renderer);
  setTextSize(textSize);

  Button startButton;
  startButton.area.w = 300;
  startButton.area.h = 50;
  startButton.area.x = 0;
  startButton.area.y = 250;
  startButton.isVisible = SDL_TRUE;

  startButton.action = &test;
  strcpy(startButton.text, "start:");

  SDL_Event event;
  SDL_bool isRunning = SDL_TRUE;

  long lastTicks = SDL_GetTicks();

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

    if (SDL_GetTicks() - lastTicks >= 1000)
    {
      lastTicks = SDL_GetTicks();
      if(timeLeft >= 1)
      {
        timeLeft--;
        updateTimer();
      } else 
      {
        strcpy(timerText, "done");
      }
    }

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    setTextSize(screenSize / strlen(timerText) * 2);
    renderText(timerText, 0, 0, screenSize);
    setTextSize(textSize);

    drawButton(renderer, &startButton);

    SDL_RenderPresent(renderer);
  }

  quitTextRenderer();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}

void drawButton(SDL_Renderer *renderer, Button *btn)
{
    if (btn->isVisible == SDL_FALSE) return;
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 100);
    SDL_RenderFillRect(renderer, &btn->area);
    renderButtonText(btn);
}

void test()
{
  timeLeft -= 30;
  updateTimer();
}

void updateTimer()
{
  int secondsLeft = timeLeft % 60;
  int minutesLeft = timeLeft / 60 % 60;
  int hoursLeft   = timeLeft / 3600;

  char secondsText[3];
  char minutesText[3];
  char hoursText[4];

  sprintf(secondsText, "%02d", secondsLeft);
  sprintf(minutesText, "%02d", minutesLeft);
  sprintf(hoursText  , "%02d", hoursLeft  );

  strcpy(timerText, hoursText);
  strcat(timerText, ":");
  strcat(timerText, minutesText);
  strcat(timerText, ":");
  strcat(timerText, secondsText);
}

int isMouseInside(SDL_FRect *rect)
{
  if (rect->x < mouseX && rect->x + rect->w > mouseX &&
      rect->y < mouseY && rect->y + rect->h > mouseY)
    return 1;
  return 0;
}

void renderButtonText(Button *btn)
{
  int textLength   = strlen(btn->text);
  int textPxLength = textSize / 2 * textLength;

  int textDrawX = btn->area.x + (btn->area.w - textPxLength) / 2;
  int textDrawY = btn->area.y + (btn->area.h - textSize    ) / 2;

  renderText(btn->text, textDrawX, textDrawY, btn->area.w);
}
 
void safeInit(int result)
{
  if (result) printf("ERROR");
}

void* safeCreate(void *result)
{
  if (result == NULL) printf("ERRORCREATION");
  return result;
}
