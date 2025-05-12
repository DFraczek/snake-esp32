#include "game_logic.h"
#include "menu.h"
#include "globals.h"
#include "game_display.h"

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

void runGame(GameState& state) {
    unsigned long now = millis();
    int yVal = analogRead(VRy);
    int xVal = analogRead(VRx);

    if (now - state.lastJoystickMovementTime > 100) {
        updateDirection(state, xVal, yVal, now);
    }

    if (now - state.lastMoveStepTime > snakeSpeed[state.speedSelection]) {
        updateSnake(state);
        state.lastMoveStepTime = now;
    }
}

void updateSnake(GameState& state) {
    eatApple(state);
    SnakeSegment tail = state.snake[state.snakeLength - 1];
    moveSnake(state);

    handleCollisions(state);
    handleWinningGame(state);
    if (!state.isGameRunning) return; 
    
    if (!state.wasAppleEaten) {
        clearOldTail(tail);
    }

    state.wasAppleEaten = false;
    renderGameObjects(state);
}

void updateDirection(GameState& state, int xVal, int yVal, unsigned long now) {
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
    state.lastJoystickMovementTime = now;
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
    return (x < WALL_X1 || x >= WALL_X2|| y < WALL_Y1 || y >= WALL_Y2);
}

bool checkSelfCollision(GameState& state) {
    int x = state.snake[0].x;
    int y = state.snake[0].y;
    for (int i = 1; i < state.snakeLength; ++i) {
        if (x == state.snake[i].x && y == state.snake[i].y) return true;
    }
    return false;
}

bool isAppleOnSnake(GameState& state, int x, int y) {
    for (int i = 0; i < state.snakeLength; ++i) {
        if (state.snake[i].x == x && state.snake[i].y == y) return true;
    }
    return false;
}

void generateApplePosition(GameState& state) {
    do {
        state.appleX = random(WALL_X1, WALL_X2); 
        state.appleY = random(WALL_Y1, WALL_Y2);
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

bool isHeadAtApplePosition(GameState& state){
  return state.snake[0].x == state.appleX && state.snake[0].y == state.appleY;
}
