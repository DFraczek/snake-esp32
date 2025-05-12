#ifndef GAME_H
#define GAME_H

#include "globals.h"

void startGame(GameState& state);
void resetGameState(GameState& state);
void initSnake(GameState& state);
void runGame(GameState& state);
void updateSnake(GameState& state);
void updateDirection(GameState& state, int xVal, int yVal, unsigned long now);
void endGameIf(bool condition, GameState& state, char* message);
void handleCollisions(GameState& state);
bool checkWallCollision(GameState& state);
bool checkSelfCollision(GameState& state);
void handleWinningGame(GameState& state);
bool isAppleOnSnake(GameState& state, int x, int y);
void generateApplePosition(GameState& state);
void moveSnake(GameState& state);
void eatApple(GameState& state);
bool isHeadAtApplePosition(GameState& state);

#endif
