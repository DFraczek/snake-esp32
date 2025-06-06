#include <SPI.h>
#include <TFT_eSPI.h>

#include "menu.h"
#include "globals.h"
#include "game_logic.h"

TFT_eSPI tft = TFT_eSPI();

GameState gameState;
const int snakeSpeed[] = {300, 200, 100};
const uint16_t COLOR_DARK_GREEN = tft.color565(6, 140, 42);
const uint16_t COLOR_DARK_RED   = tft.color565(192, 49, 44);

void setup() {
  uint32_t seed = esp_random();
  randomSeed(seed);
  Serial.begin(115200);
  pinMode(SW, INPUT_PULLUP);
  tft.init();
  tft.setRotation(0);
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
