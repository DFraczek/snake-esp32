#ifndef GLOBALS_H
#define GLOBALS_H

#include <TFT_eSPI.h>
#include <deque>

#define COLOR_BLACK TFT_BLACK
#define COLOR_GREEN TFT_GREEN
#define COLOR_RED TFT_RED
extern const uint16_t COLOR_DARK_GREEN;
extern const uint16_t COLOR_DARK_RED;

#define VRx 34 //JOYSTICK VRX
#define VRy 35 //JOYSTICK VRY
#define SW  25 //JOYSTICK SW

#define MAX_SNAKE_LENGHT 130

#define WALL_Y1 2
#define WALL_Y2 15
#define WALL_X1 1
#define WALL_X2 11

#define SCORE_X 130
#define SCORE_Y 10

#define SEGMENT_SIZE 20

#define JOY_THRESHOLD_HIGH  3500
#define JOY_THRESHOLD_LOW  1000

extern const int snakeSpeed[];
extern TFT_eSPI tft;

struct SnakeSegment {
  int x;
  int y;
};

struct SnakeData {
    std::deque<SnakeSegment> body;
    int dirX, dirY;
    int lastDirX, lastDirY;
};

struct AppleData{
  int x, y;
  bool wasAppleEaten;
};

struct TimingData {
    unsigned long lastJoystickMovementTime = 0;
    unsigned long lastMoveStepTime = 0;
};

struct GameState {
    SnakeData snake;
    AppleData apple;
    TimingData timing;
    int score;
    bool isGameRunning;
    int speedSelection = 0;
}; 

#endif
