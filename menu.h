#ifndef MENU_H
#define MENU_H

#include "globals.h"

void drawMenuArrow();
void drawMenu(GameState& gameState);
void handleMenuClick(GameState& gameState);
void updateMenuSelection(unsigned long currentTime);
void updateSpeedSelection(GameState& gameState, unsigned long currentTime);
void updateSpeedDisplay(GameState& gameState);
void handleMenuNavigation(GameState& gameState);

#endif
