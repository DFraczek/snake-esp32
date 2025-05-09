#ifndef GLOBALS_H
#define GLOBALS_H

#include <TFT_eSPI.h>

#define COLOR_BLACK TFT_BLACK
#define COLOR_GREEN TFT_GREEN
#define COLOR_RED TFT_RED
#define COLOR_YELLOW TFT_YELLOW

#define VRx 34
#define VRy 35
#define SW  25

#define MAX_SNAKE_LENGHT 168
#define GAMEBOARD_WIDTH 12
#define GAMEBOARD_HEIGHT 16
#define SCORE_X 130
#define SCORE_Y 10
#define SEGMENT_SIZE 20
#define JOY_THRESHOLD_HIGH  3500
#define JOY_THRESHOLD_LOW  1000

struct SnakeSegment {
  int x;
  int y;
};

struct GameState {
    int dirX = 1, dirY = 0;
    int lastDirX = 1, lastDirY = 0;
    int score = 0;
    int snakeLength = 3;
    SnakeSegment snake[MAX_SNAKE_LENGHT];
    int appleX = 0, appleY = 0;
    bool wasAppleEaten = false;
    bool isGameRunning = false;
    unsigned long lastInputTime = 0;
    unsigned long lastMoveStepTime = 0;
    int speedSelection = 0;
};

extern const int snakeSpeed[];
extern TFT_eSPI tft; 

#endif
