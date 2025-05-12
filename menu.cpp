#include "menu.h"
#include "globals.h"
#include "game_logic.h"

#include "img/apple_image.h"

static const char* snakeSpeedText[] = {"SLOW", "NORMAL", "FAST"};
static int menuSelection = 0;
static unsigned long lastMoveTime = 0;
static bool wasGameSpeedChanged = false;
static int lastSwState = HIGH;

void drawMenu(GameState& gameState) {
  tft.fillScreen(COLOR_BLACK);
  tft.drawRect(0,40,240,280,COLOR_DARK_GREEN);

  tft.setTextSize(3);
  tft.setTextColor(COLOR_DARK_GREEN);
  tft.setCursor(80, 10);
  tft.print("SNAKE");

  tft.setTextSize(2);
  tft.drawRect(60, 80, 160, 40, COLOR_DARK_GREEN);
  tft.setCursor(95, 93);
  tft.setTextColor(COLOR_DARK_RED);
  tft.print("NEW GAME");

  tft.drawRect(60, 160, 160, 40, COLOR_DARK_GREEN);
  tft.setCursor(70, 173);
  tft.print("<");
  tft.setCursor(200, 173);
  tft.print(">");
  if(gameState.speedSelection == 1){ tft.setCursor(110, 173); }
  else{ tft.setCursor(120, 173); } 
  tft.print(snakeSpeedText[gameState.speedSelection]);
}

void drawMenuArrow() {
  tft.fillRect(15,90,20,20,COLOR_BLACK);
  tft.fillRect(15,170,20,20,COLOR_BLACK);

  if (menuSelection == 0) {
    tft.pushImage(15, 90, 20, 20, apple_image);
  } else {
    tft.pushImage(15, 170, 20, 20, apple_image);
  }
}

void handleMenuNavigation(GameState& gameState) {
  unsigned long currentTime = millis();
  updateMenuSelection(currentTime);
  updateSpeedSelection(gameState, currentTime);
  updateSpeedDisplay(gameState);
}

void updateMenuSelection(unsigned long currentTime) {
  int yVal = analogRead(VRy);
  if (currentTime - lastMoveTime > 200) {
    if (yVal > JOY_THRESHOLD_HIGH) {
      menuSelection = 1;
      drawMenuArrow();
      lastMoveTime = currentTime;
    } else if (yVal < JOY_THRESHOLD_LOW) {
      menuSelection = 0;
      drawMenuArrow();
      lastMoveTime = currentTime;
    }
  }
}

void updateSpeedSelection(GameState& gameState, unsigned long currentTime) {
  int xVal = analogRead(VRx);
  if (menuSelection == 1 && currentTime - lastMoveTime > 200) {
    if (xVal > JOY_THRESHOLD_HIGH) {
      gameState.speedSelection = (gameState.speedSelection + 1) % 3;
      lastMoveTime = currentTime;
      wasGameSpeedChanged = true;
    } else if (xVal < JOY_THRESHOLD_LOW) {
      gameState.speedSelection = (gameState.speedSelection + 2) % 3;
      lastMoveTime = currentTime;
      wasGameSpeedChanged = true;
    }
  }
}

void handleMenuClick(GameState& gameState) {
  int sw = digitalRead(SW);
  if (sw == LOW && lastSwState == HIGH) {
    delay(50);
    if (menuSelection == 0) {
      startGame(gameState);
      gameState.isGameRunning = true;
    }
  }
  lastSwState = sw;
}

void updateSpeedDisplay(GameState& gameState) {
  if (wasGameSpeedChanged) {
    tft.fillRect(95, 173, 100, 25, COLOR_BLACK);
    tft.setTextSize(2);
    if (gameState.speedSelection == 1) tft.setCursor(110, 173);
    else tft.setCursor(120, 173);
    tft.print(snakeSpeedText[gameState.speedSelection]);
    wasGameSpeedChanged = false;
  }
}
