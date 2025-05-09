#include "game.h"
#include "menu.h"
#include "globals.h"

void startGame(GameState& state) {
    resetGameState(state);
    initSnake(state);
    drawInitialGameBoard(state);
    generateApplePosition(state);
    drawApple(state);
    drawSnake(state);
}

void resetGameState(GameState& state) {
    state.snakeLength = 3;
    state.dirX = 1;
    state.dirY = 0;
    state.lastDirX = 1;
    state.lastDirY = 0;
    state.wasAppleEaten = false;
    state.score = 0;
}

void initSnake(GameState& state) {
    state.snake[0] = {6, 7};
    state.snake[1] = {5, 7};
    state.snake[2] = {4, 7};
}

void drawInitialGameBoard(GameState& state) {
    tft.fillScreen(COLOR_BLACK);
    tft.pushImage(100, 10, SEGMENT_SIZE, SEGMENT_SIZE, apple_image);
    tft.setTextSize(3);
    tft.setTextColor(tft.color565(6, 140, 42));
    tft.setCursor(SCORE_X, SCORE_Y);
    tft.print(state.score);
}

void runGame(GameState& state) {
    unsigned long now = millis();
    int yVal = analogRead(VRy);
    int xVal = analogRead(VRx);

    if (shouldUpdateDirection(state, now)) {
        updateDirection(state, xVal, yVal, now);
    }

    if (shouldMoveSnake(state, now)) {
        updateSnake(state);
        handleCollisions(state);
        handleWinningGame(state);
        state.lastMoveStepTime = now;
    }
}

bool shouldUpdateDirection(GameState& state, unsigned long now) {
    return (now - state.lastInputTime > 100);
}

void updateDirection(GameState& state, int xVal, int yVal, unsigned long now) {
    updateSnakeDirectionFromJoystick(yVal, xVal, state);
    state.lastInputTime = now;
}

bool shouldMoveSnake(GameState& state, unsigned long now) {
    return (now - state.lastMoveStepTime > snakeSpeed[state.speedSelection]);
}

void updateSnake(GameState& state) {
    eatApple(state);
    SnakeSegment tail = state.snake[state.snakeLength - 1];
    moveSnake(state);

    if (!state.wasAppleEaten) {
        clearOldTail(tail);
    }

    state.wasAppleEaten = false;
    renderGameObjects(state);
}

void clearOldTail(SnakeSegment& tail) {
    tft.fillRect(tail.x * 20, tail.y * 20, 20, 20, COLOR_BLACK);
}

void renderGameObjects(GameState& state) {
    drawApple(state);
    drawSnake(state);
    drawGameFrame();
}

void drawGameFrame() {
    tft.drawRect(0, 40, 240, 280, tft.color565(6, 140, 42));
}

void endGameIf(bool condition, GameState& state, char* message) {
    if (condition) {
        state.isGameRunning = false;
        showEndGameView(state.score, message);
        drawMenu(state);
        drawMenuArrow();
    }
}

void handleCollisions(GameState& state) {
    endGameIf(checkSelfCollision(state) || checkWallCollision(state), state, "GAME OVER");
}

void handleWinningGame(GameState& state) {
    endGameIf(state.snakeLength == MAX_SNAKE_LENGHT, state, "CONGRATS!");
}

bool checkWallCollision(GameState& state) {
    int x = state.snake[0].x;
    int y = state.snake[0].y;
    return (x < 0 || x >= GAMEBOARD_WIDTH || y < 2 || y >= GAMEBOARD_HEIGHT);
}

bool checkSelfCollision(GameState& state) {
    int x = state.snake[0].x;
    int y = state.snake[0].y;
    for (int i = 1; i < state.snakeLength; ++i) {
        if (x == state.snake[i].x && y == state.snake[i].y) return true;
    }
    return false;
}

void drawSnake(GameState& state) {
    for (int i = 0; i < state.snakeLength; ++i) {
        int x = state.snake[i].x * SEGMENT_SIZE;
        int y = state.snake[i].y * SEGMENT_SIZE;

        if (i == 0) {
            drawSnakeHead(x, y, state.dirX, state.dirY);
        } else if (i == state.snakeLength - 1) {
            drawSnakeTail(x, y, state.snake[i - 1], state.snake[i]);
        } else {
            drawSnakeBody(x, y, state.snake[i - 1], state.snake[i + 1], state.snake[i]);
        }
    }
}

void drawSnakeHead(int x, int y, int dirX, int dirY) {
    const uint16_t* img =
        (dirX == 1) ? head_right_image :
        (dirX == -1) ? head_left_image :
        (dirY == 1) ? head_down_image :
        (dirY == -1) ? head_up_image :
        nullptr;

    drawSegmentImage(x, y, img);
}

void drawSnakeTail(int x, int y, SnakeSegment& prev, SnakeSegment& tail) {
    int dx = prev.x - tail.x;
    int dy = prev.y - tail.y;

    const uint16_t* img =
        (dx == 1) ? tail_right_image :
        (dx == -1) ? tail_left_image :
        (dy == 1) ? tail_down_image :
        (dy == -1) ? tail_up_image :
        nullptr;

    drawSegmentImage(x, y, img);
}

void drawSnakeBody(int x, int y, SnakeSegment& prev, SnakeSegment& next, SnakeSegment& current) {
    int dx1 = prev.x - current.x;
    int dy1 = prev.y - current.y;
    int dx2 = next.x - current.x;
    int dy2 = next.y - current.y;

    const uint16_t* img = nullptr;

    if (dx1 == 0 && dx2 == 0) img = body_vertical_image;
    else if (dy1 == 0 && dy2 == 0) img = body_horizontal_image;
    else if ((dy1 == -1 && dx2 == 1) || (dy2 == -1 && dx1 == 1)) img = body_corner_bottomleft_image;
    else if ((dx1 == 1 && dy2 == 1) || (dx2 == 1 && dy1 == 1)) img = body_corner_topleft_image;
    else if ((dy1 == 1 && dx2 == -1) || (dy2 == 1 && dx1 == -1)) img = body_corner_topright_image;
    else if ((dx1 == -1 && dy2 == -1) || (dx2 == -1 && dy1 == -1)) img = body_corner_bottomright_image;

    drawSegmentImage(x, y, img);
}

void drawSegmentImage(int x, int y, const uint16_t* img) {
    if (img) {
        tft.pushImage(x, y, SEGMENT_SIZE, SEGMENT_SIZE, img);
    } else {
        tft.fillRect(x, y, SEGMENT_SIZE, SEGMENT_SIZE, COLOR_GREEN);
    }
}

void drawApple(GameState& state) {
    if (apple_image) {
        tft.pushImage(state.appleX * SEGMENT_SIZE, state.appleY * SEGMENT_SIZE,SEGMENT_SIZE, SEGMENT_SIZE, apple_image);
    } else {
        tft.fillRect(state.appleX * SEGMENT_SIZE, state.appleY * SEGMENT_SIZE,SEGMENT_SIZE, SEGMENT_SIZE, COLOR_RED);
    }
}

bool isAppleOnSnake(GameState& state, int x, int y) {
    for (int i = 0; i < state.snakeLength; ++i) {
        if (state.snake[i].x == x && state.snake[i].y == y) return true;
    }
    return false;
}

void generateApplePosition(GameState& state) {
    do {
        state.appleX = random(0, GAMEBOARD_WIDTH);
        state.appleY = random(2, GAMEBOARD_HEIGHT);
    } while (isAppleOnSnake(state, state.appleX, state.appleY));
}

void moveSnake(GameState& state) {
    if (state.wasAppleEaten) state.snakeLength++;
    for (int i = state.snakeLength - 1; i > 0; --i)
        state.snake[i] = state.snake[i - 1];

    state.snake[0].x += state.dirX;
    state.snake[0].y += state.dirY;
    state.lastDirX = state.dirX;
    state.lastDirY = state.dirY;
}

void eatApple(GameState& state) {
    if (isHeadAtApplePosition(state)) {
        state.wasAppleEaten = true;
        clearOldApple(state);
        state.score++;
        updateScore(state);
        generateApplePosition(state);
    }
}

void clearOldApple(GameState& state){
  tft.fillRect(state.appleX * SEGMENT_SIZE, state.appleY * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE, COLOR_BLACK);
}

bool isHeadAtApplePosition(GameState& state){
  return state.snake[0].x == state.appleX && state.snake[0].y == state.appleY;
}

void updateScore(GameState& state) {
    tft.fillRect(SCORE_X, SCORE_Y, 50, 25, COLOR_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(tft.color565(6,140,42));
    tft.setCursor(SCORE_X, SCORE_Y);
    tft.print(state.score);
}

void updateSnakeDirectionFromJoystick(int yVal, int xVal, GameState& state) {
    int newDirX = state.dirX;
    int newDirY = state.dirY;

    if (yVal > JOY_THRESHOLD_HIGH) {
        newDirX = 0;
        newDirY = 1;
    } else if (yVal < JOY_THRESHOLD_LOW) {
        newDirX = 0;
        newDirY = -1;
    } else if (xVal > JOY_THRESHOLD_HIGH) {
        newDirX = 1;
        newDirY = 0;
    } else if (xVal < JOY_THRESHOLD_LOW) {
        newDirX = -1;
        newDirY = 0;
    }

    bool isReverse = (newDirX == -state.lastDirX && newDirY == -state.lastDirY);
    if (!isReverse) {
        state.dirX = newDirX;
        state.dirY = newDirY;
    }
}

void showEndGameView(int score, char* message) {
  tft.fillScreen(COLOR_BLACK);
  tft.drawRect(0,0,240,320,tft.color565(6,140,42));
  tft.setTextColor(tft.color565(6,140,42));
  tft.setTextSize(3);
  tft.setCursor(45, 110);
  tft.print(message);

  tft.setTextSize(2);
  tft.setCursor(50, 160);
  tft.setTextColor(tft.color565(192,49,44));
  tft.print("Score: ");
  tft.pushImage(130, 155, 20, 20, apple_image);
  tft.setTextSize(3);
  tft.setTextColor(tft.color565(6,140,42));
  tft.setCursor(160, 155);
  tft.print(score);

  delay(5000);
}
