#include "globals.h"
#include "game_display.h"

void drawInitialGameBoard(GameState& state) {
    tft.fillScreen(COLOR_BLACK);
    tft.pushImage(100, 10, SEGMENT_SIZE, SEGMENT_SIZE, apple_image);
    tft.setTextSize(3);
    tft.setTextColor(COLOR_DARK_GREEN);
    tft.setCursor(SCORE_X, SCORE_Y);
    tft.print(state.score);
    drawGameFrame();
}

void drawGameFrame() {
    tft.drawRect(19, 39, 203, 263, COLOR_DARK_GREEN);
}

void clearOldTail(SnakeSegment& tail) {
    tft.fillRect(tail.x * 20, tail.y * 20, 20, 20, COLOR_BLACK);
}

void renderGameObjects(GameState& state) {
    drawApple(state);
    drawSnake(state);
}

void drawSnake(GameState& state) {
    size_t snakeLength = state.snake.body.size();
    for (size_t i = 0; i < snakeLength; i++) {
        int x = state.snake.body[i].x * SEGMENT_SIZE;
        int y = state.snake.body[i].y * SEGMENT_SIZE;

        if (i == 0) {
            drawSnakeHead(x, y, state.snake.dirX, state.snake.dirY);
        } else if (i == snakeLength - 1) {
            drawSnakeTail(x, y, state.snake.body[i - 1], state.snake.body[i]);
        } else {
            drawSnakeBody(x, y, state.snake.body[i - 1], state.snake.body[i + 1], state.snake.body[i]);
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
    int x = state.apple.x * SEGMENT_SIZE;
    int y = state.apple.y * SEGMENT_SIZE;
    if (apple_image) {
        tft.pushImage(x, y, SEGMENT_SIZE, SEGMENT_SIZE, apple_image);
    } else {
        tft.fillRect(x, y, SEGMENT_SIZE, SEGMENT_SIZE, COLOR_RED);
    }
}

void clearOldApple(GameState& state){
  int x = state.apple.x * SEGMENT_SIZE;
  int y = state.apple.y * SEGMENT_SIZE;
  tft.fillRect(x, y, SEGMENT_SIZE, SEGMENT_SIZE, COLOR_BLACK);
}

void updateScore(GameState& state) {
    tft.fillRect(SCORE_X, SCORE_Y, 50, 25, COLOR_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(COLOR_DARK_GREEN);
    tft.setCursor(SCORE_X, SCORE_Y);
    tft.print(state.score);
}

void showEndGameView(int score, char* message) {
  tft.fillScreen(COLOR_BLACK);
  tft.drawRect(0,0,240,320,COLOR_DARK_GREEN);
  tft.setTextColor(COLOR_DARK_GREEN);
  tft.setTextSize(3);
  tft.setCursor(45, 110);
  tft.print(message);

  tft.setTextSize(2);
  tft.setCursor(50, 160);
  tft.setTextColor(COLOR_DARK_RED);
  tft.print("Score: ");
  tft.pushImage(130, 155, 20, 20, apple_image);
  tft.setTextSize(3);
  tft.setTextColor(COLOR_DARK_GREEN);
  tft.setCursor(160, 155);
  tft.print(score);

  delay(5000);
}
