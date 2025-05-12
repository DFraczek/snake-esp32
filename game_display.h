#ifndef DISPLAY_H
#define DISPLAY_H

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

void drawInitialGameBoard(GameState& state);
void drawGameFrame();
void clearOldTail(SnakeSegment& tail);
void renderGameObjects(GameState& state);
void drawSnake(GameState& state);
void drawSnakeHead(int x, int y, int dirX, int dirY);
void drawSnakeTail(int x, int y, SnakeSegment& prev, SnakeSegment& tail);
void drawSnakeBody(int x, int y, SnakeSegment& prev, SnakeSegment& next, SnakeSegment& current);
void drawSegmentImage(int x, int y, const uint16_t* img);
void drawApple(GameState& state);
void clearOldApple(GameState& state);
void updateScore(GameState& state);
void showEndGameView(int score, char* message);

#endif
