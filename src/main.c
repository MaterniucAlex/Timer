#include <stdio.h>
#include <string.h>
#include "SDL3/SDL.h"
#include "textRenderer/textRenderer.h"

enum state {
  SETTING,
  RUNNING
};

typedef struct {
  SDL_FRect area;
  char text[21];
  void (*action)();
  SDL_bool isVisible;
} Button;

const int textSize = 32;
const int screenSize = 320;
const int padding = screenSize / 10;

enum state currentState = SETTING;

float mouseX, mouseY;
int timeLeft = 0;
char timerText[16] = "00:00:00";
SDL_bool isTimerPaused = SDL_FALSE;
#define nrOfButtons 9
Button buttonsList[nrOfButtons];
int timerTextSize, timerTextY;

void minuteUp();
void minuteDown();
void hourUp();
void hourDown();
void secondUp();
void secondDown();
void pause();
void reset();
void safeInit(int result);
void*safeCreate(void *result);
void renderButtonText(Button *btn);
void drawButton(SDL_Renderer *renderer, Button *btn);
int  isMouseInside(SDL_FRect *rect);
void updateTimer();
void changeStateToRunning();
void changeStateToSetting();
void createButtons();

int main()
{

  safeInit(SDL_Init(SDL_INIT_VIDEO));
  safeInit(!IMG_Init(IMG_INIT_PNG));

  SDL_Window   *window   = safeCreate(SDL_CreateWindow("Timer", screenSize, screenSize, 0));
  SDL_Renderer *renderer = safeCreate(SDL_CreateRenderer(window, NULL));
  SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

  initTextRenderer(renderer);
  setTextSize(textSize);

  updateTimer();

  createButtons();

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
          for(int i = 0; i < nrOfButtons; i++)
          {
            if (isMouseInside(&buttonsList[i].area) && buttonsList[i].isVisible == SDL_TRUE)
              buttonsList[i].action();
          }
          break;
      }
    }

    if (SDL_GetTicks() - lastTicks >= 1000)
    {
      lastTicks = SDL_GetTicks();
      if(timeLeft >= 1 && currentState == RUNNING && isTimerPaused == SDL_FALSE)
      {
        timeLeft--;
        updateTimer();
      }
      if(timeLeft <= 0 && currentState == RUNNING) {
        changeStateToSetting();
      }
    }

    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_RenderClear(renderer);

    setTextSize(timerTextSize);
    renderText(timerText, padding, timerTextY, screenSize - padding * 2);
    setTextSize(textSize);

    for(int i = 0; i < nrOfButtons; i++)
    {
      drawButton(renderer, &buttonsList[i]);
    }

    SDL_RenderPresent(renderer);
  }

  quitTextRenderer();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void drawButton(SDL_Renderer *renderer, Button *btn)
{
  if (btn->isVisible == SDL_FALSE) return;

  SDL_SetRenderDrawColor(renderer, 31, 31, 31, 100);
  SDL_RenderFillRect(renderer, &btn->area);
  renderButtonText(btn);
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 100);
  SDL_RenderRect(renderer, &btn->area);
}

void updateTimer()
{
  if (timeLeft < 0) timeLeft = 0;

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

  int buttonTextSize;
  
  if (textPxLength >= btn->area.w)
  {
    buttonTextSize = btn->area.w / textLength;
    int textPxLength = buttonTextSize / 2 * textLength;
  }
  else buttonTextSize = textSize;

  int textDrawX = btn->area.x + (btn->area.w - textPxLength)   / 2;
  int textDrawY = btn->area.y + (btn->area.h - buttonTextSize) / 2;

  setTextSize(buttonTextSize);
  renderText(btn->text, textDrawX, textDrawY, btn->area.w);
  setTextSize(textSize);
}

void safeInit(int result)
{
  if (result)
  {
    printf("ERROR");
    SDL_Quit();
  }
}

void* safeCreate(void *result)
{
  if (result == NULL)
  {
    printf("ERROR CREATING");
    SDL_Quit();
  }
  return result;
}

void minuteUp()
{
  timeLeft += 60;
  updateTimer();
}

void minuteDown()
{
    timeLeft -= 60;
  updateTimer();
}

void hourUp()
{
  timeLeft += 3600;
  updateTimer();
}

void hourDown()
{
    timeLeft -= 3600;
  updateTimer();
}

void secondUp()
{
  timeLeft += 1;
  updateTimer();
}

void secondDown()
{
    timeLeft -= 1;
  updateTimer();
}

void pause()
{
  if (isTimerPaused == SDL_TRUE)
  {
    isTimerPaused = SDL_FALSE;
    strcpy(buttonsList[7].text, "pause");
  }
  else {
    isTimerPaused = SDL_TRUE;
    strcpy(buttonsList[7].text, "resume");
  }
}

void reset()
{
  timeLeft = 0;
  updateTimer();
  changeStateToSetting();
  strcpy(buttonsList[7].text, "pause");
}

void changeStateToSetting()
{
  currentState = SETTING;
  for(int i = 0; i < 7; i++)
    buttonsList[i].isVisible = SDL_TRUE;
  buttonsList[7].isVisible = SDL_FALSE;
  buttonsList[8].isVisible = SDL_FALSE;
}

void changeStateToRunning()
{
  if (timeLeft == 0) return;
  currentState = RUNNING;
  for(int i = 0; i < 7; i++)
    buttonsList[i].isVisible = SDL_FALSE;
  buttonsList[7].isVisible = SDL_TRUE;
  buttonsList[8].isVisible = SDL_TRUE;
  isTimerPaused = SDL_FALSE;
}

void createButtons()
{
  Button startButton;
  startButton.area.w = screenSize - padding * 2;
  startButton.area.h = padding * 2;
  startButton.area.x = padding;
  startButton.area.y = screenSize - startButton.area.h - padding;
  startButton.isVisible = SDL_TRUE;
  startButton.action = &changeStateToRunning;
  strcpy(startButton.text, "start:");

  timerTextSize = (screenSize - padding * 2) / strlen(timerText) * 2;
  timerTextY    = (screenSize - padding - startButton.area.h - timerTextSize) / 2;

  Button minutesUp;
  minutesUp.isVisible = SDL_TRUE;
  minutesUp.area.w = padding * 2;
  minutesUp.area.h = padding;
  minutesUp.area.x = (screenSize - minutesUp.area.w) / 2;
  minutesUp.area.y = timerTextY - minutesUp.area.h;
  minutesUp.action = &minuteUp;
  strcpy(minutesUp.text, "^");

  Button minutesDown;
  minutesDown.isVisible = SDL_TRUE;
  minutesDown.area.w = padding * 2;
  minutesDown.area.h = padding;
  minutesDown.area.x = (screenSize - minutesUp.area.w) / 2;
  minutesDown.area.y = timerTextY + timerTextSize;
  minutesDown.action = &minuteDown;
  strcpy(minutesDown.text, "-");

  Button hoursUp;
  hoursUp.isVisible = SDL_TRUE;
  hoursUp.area.w = padding * 2;
  hoursUp.area.h = padding;
  hoursUp.area.x = minutesUp.area.x - padding - hoursUp.area.w;
  hoursUp.area.y = timerTextY - minutesDown.area.h;
  hoursUp.action = &hourUp;
  strcpy(hoursUp.text, "^");

  Button hoursDown;
  hoursDown.isVisible = SDL_TRUE;
  hoursDown.area.w = padding * 2;
  hoursDown.area.h = padding;
  hoursDown.area.x = minutesUp.area.x - padding - hoursUp.area.w;
  hoursDown.area.y = timerTextY + timerTextSize;
  hoursDown.action = &hourDown;
  strcpy(hoursDown.text, "-");

  Button secondsUp;
  secondsUp.isVisible = SDL_TRUE;
  secondsUp.area.w = padding * 2;
  secondsUp.area.h = padding;
  secondsUp.area.x = minutesUp.area.x + minutesUp.area.w + padding;
  secondsUp.area.y = timerTextY - minutesUp.area.h;
  secondsUp.action = &secondUp;
  strcpy(secondsUp.text, "^");

  Button secondsDown;
  secondsDown.isVisible = SDL_TRUE;
  secondsDown.area.w = padding * 2;
  secondsDown.area.h = padding;
  secondsDown.area.x = minutesDown.area.x + minutesUp.area.w + padding;
  secondsDown.area.y = timerTextY + timerTextSize;
  secondsDown.action = &secondDown;
  strcpy(secondsDown.text, "-");

  Button pauseButton;
  pauseButton.area.w = (screenSize - padding * 3) / 2;
  pauseButton.area.h = padding;
  pauseButton.area.x = padding;
  pauseButton.area.y = screenSize - startButton.area.h - padding;
  pauseButton.isVisible = SDL_FALSE;
  pauseButton.action = &pause;
  strcpy(pauseButton.text, "pause");

  Button resetButton;
  resetButton.area.w = (screenSize - padding * 3) / 2;
  resetButton.area.h = padding;
  resetButton.area.x = screenSize - padding - resetButton.area.w;
  resetButton.area.y = screenSize - startButton.area.h - padding;
  resetButton.isVisible = SDL_FALSE;
  resetButton.action = &reset;
  strcpy(resetButton.text, "reset");

  buttonsList[0] = startButton;
  buttonsList[1] = minutesUp;
  buttonsList[2] = minutesDown;
  buttonsList[3] = hoursUp;
  buttonsList[4] = hoursDown;
  buttonsList[5] = secondsUp;
  buttonsList[6] = secondsDown;
  buttonsList[7] = pauseButton;
  buttonsList[8] = resetButton;
}
