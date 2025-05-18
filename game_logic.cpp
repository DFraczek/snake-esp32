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
    state.snake.dirX = 1;
    state.snake.dirY = 0;
    state.snake.lastDirX = 1;
    state.snake.lastDirY = 0;
    state.apple.wasAppleEaten = false;
    state.score = 0;
}

void initSnake(GameState& state) {
    state.snake.body.clear(); 
    state.snake.body.push_back({6, 7});
    state.snake.body.push_back({5, 7});
    state.snake.body.push_back({4, 7}); 
}

void runGame(GameState& state) {
    unsigned long now = millis();
    int yVal = analogRead(VRy);
    int xVal = analogRead(VRx);

    if (now - state.timing.lastJoystickMovementTime > 100) {
        updateDirection(state, xVal, yVal, now);
    }

    if (now - state.timing.lastMoveStepTime > snakeSpeed[state.speedSelection]) {
        updateSnake(state);
        state.timing.lastMoveStepTime = now;
    }
}

void updateSnake(GameState& state) {
    eatApple(state);
    moveSnake(state);

    handleCollisions(state);
    handleWinningGame(state);
    if (!state.isGameRunning) return; 

    renderGameObjects(state);
}

void updateDirection(GameState& state, int xVal, int yVal, unsigned long now) {
    int newDirX, newDirY;

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
    } else{
        newDirX = state.snake.dirX;
        newDirY = state.snake.dirY;
    }
    
    bool isReverse = (newDirX == -state.snake.lastDirX && newDirY == -state.snake.lastDirY);
    if (!isReverse) {
        state.snake.dirX = newDirX;
        state.snake.dirY = newDirY;
    }
    state.timing.lastJoystickMovementTime = now;
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

bool checkWallCollision(GameState& state) {
    int x = state.snake.body.front().x;
    int y = state.snake.body.front().y;
    return (x < WALL_X1 || x >= WALL_X2|| y < WALL_Y1 || y >= WALL_Y2);
}

bool checkSelfCollision(GameState& state) {
    int x = state.snake.body.front().x;
    int y = state.snake.body.front().y;
    for (size_t i = 1; i < state.snake.body.size(); i++) {
        if (x == state.snake.body[i].x && y == state.snake.body[i].y) return true;
    }
    return false;
}

void handleWinningGame(GameState& state) {
    size_t snakeLength = state.snake.body.size();
    endGameIf(snakeLength == MAX_SNAKE_LENGHT, state, "CONGRATS!");
}

void moveSnake(GameState& state) {
    SnakeSegment newHead = {
        state.snake.body.front().x + state.snake.dirX,
        state.snake.body.front().y + state.snake.dirY
    };

    state.snake.body.push_front(newHead);
    
    if (!state.apple.wasAppleEaten) {
        SnakeSegment tail = state.snake.body.back();
        clearOldTail(tail);
        state.snake.body.pop_back();
    }else{
        state.apple.wasAppleEaten = false;
    }

    state.snake.lastDirX = state.snake.dirX;
    state.snake.lastDirY = state.snake.dirY;
}

void eatApple(GameState& state) {
    if (isHeadAtApplePosition(state)) {
        state.apple.wasAppleEaten = true;
        clearOldApple(state);
        state.score++;
        updateScore(state);
        generateApplePosition(state);
    }
}

bool isHeadAtApplePosition(GameState& state) {
    return state.snake.body.front().x == state.apple.x && state.snake.body.front().y == state.apple.y;
}

void generateApplePosition(GameState& state) {
    do {
        state.apple.x = random(WALL_X1, WALL_X2); 
        state.apple.y = random(WALL_Y1, WALL_Y2);
    } while (isAppleOnSnake(state, state.apple.x, state.apple.y));
}

bool isAppleOnSnake(GameState& state, int x, int y) {
    for (size_t i = 0; i < state.snake.body.size(); i++) {
        if (state.snake.body[i].x == x && state.snake.body[i].y == y) return true;
    }
    return false;
    
}
