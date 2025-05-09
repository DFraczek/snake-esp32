#include <SPI.h>
#include <TFT_eSPI.h>

#include "menu.h"
#include "globals.h"
#include "game.h"

TFT_eSPI tft = TFT_eSPI();

GameState gameState;
const int snakeSpeed[] = {300, 200, 100};

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  pinMode(SW, INPUT_PULLUP);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(COLOR_BLACK);
  drawMenu(gameState);
  drawMenuArrow();
}

void loop() {
  if (!gameState.isGameRunning) {
    handleMenuNavigation(gameState);
    handleMenuClick(gameState);
  } else {
    runGame(gameState);
  }
}
