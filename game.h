#ifndef GAME_H
#define GAME_H

#include "globals.h"

#include "img/head/head_up_image.h"
#include "img/head/head_down_image.h"
#include "img/head/head_left_image.h"
#include "img/head/head_right_image.h"

#include "img/tail/tail_up_image.h"
#include "img/tail/tail_down_image.h"
#include "img/tail/tail_left_image.h"
#include "img/tail/tail_right_image.h"

#include "img/body/body_vertical_image.h"
#include "img/body/body_horizontal_image.h"

#include "img/body/body_corner_topleft_image.h"
#include "img/body/body_corner_topright_image.h"
#include "img/body/body_corner_bottomleft_image.h"
#include "img/body/body_corner_bottomright_image.h"

#include "img/apple_image.h"

void startGame(GameState& state);
void resetGameState(GameState& state);
void initSnake(GameState& state);
void drawInitialGameBoard(GameState& state);
void runGame(GameState& state);
bool shouldUpdateDirection(GameState& state, unsigned long now);
void updateDirection(GameState& state, int xVal, int yVal, unsigned long now);
bool shouldMoveSnake(GameState& state, unsigned long now);
void updateSnake(GameState& state);
void clearOldTail(SnakeSegment& tail);
void renderGameObjects(GameState& state);
void drawGameFrame();
void endGameIf(bool condition, GameState& state, char* message);
void handleCollisions(GameState& state);
bool checkWallCollision(GameState& state);
bool checkSelfCollision(GameState& state);
void handleWinningGame(GameState& state);
void drawSnake(GameState& state);
void drawSnakeHead(int x, int y, int dirX, int dirY);
void drawSnakeTail(int x, int y, SnakeSegment& prev, SnakeSegment& tail);
void drawSnakeBody(int x, int y, SnakeSegment& prev, SnakeSegment& next, SnakeSegment& current);
void drawSegmentImage(int x, int y, const uint16_t* img);
void drawApple(GameState& state);
bool isAppleOnSnake(GameState& state, int x, int y);
void generateApplePosition(GameState& state);
void moveSnake(GameState& state);
void eatApple(GameState& state);
bool isHeadAtApplePosition(GameState& state);
void clearOldApple(GameState& state);
void updateScore(GameState& state);
void updateSnakeDirectionFromJoystick(int yVal, int xVal, GameState& state);
void showEndGameView(int score, char* message);

#endif
