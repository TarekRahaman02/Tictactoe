#include <cstring>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>

#include <app.hpp>
#include <context.hpp>

using namespace std;

const int width = 400;
const int height = 400;

enum CellState { empty, Player1, Player2 };

struct Cell {
  int x;
  int y;
  int h;
  int w;
  CellState state;
};

class MyApp : public uwe::App {
private:
  Cell gameState[3][3];
  int currentPlayer = 1;
  // Board dimensions
  const int boardWidth = 300;
  const int boardHeight = 300;
  int gameRunning; // 0 not running, 1 running, 2 ended
  // Cell dimensions
  const int cellSize = 100;
  const int cellPadding = 0;
  int boardX;
  int boardY;
  uwe::Font font15_;
  int winner = 0;

private:
  void initializeGame();
  void updateResult();
  void drawTicTacToeBoard();

public:
  MyApp(int width, int height, std::string title);
  ~MyApp();

  void begin() override;
  void update() override;
  void draw() override;

  void key_pressed(uwe::Scancode scancode, bool repeat) override;

  void mouse_pressed(int x, int y, uwe::Button button) override;
  void mouse_released(int x, int y, uwe::Button button) override;
  void mouse_moved(int x, int y) override;
};

MyApp::MyApp(int width, int height, std::string title) {
  init(width, height, title);
}

MyApp::~MyApp() {}

void MyApp::begin() {
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) ||               \
                         (defined(__APPLE__) && defined(__MACH__)))
  font15_ = create_font("./assets/fonts/FreeSans.ttf", 15, uwe::Colour::red());
#else
  font15_ = create_font("../assets/fonts/FreeSans.ttf", 15, uwe::Colour::red());
#endif
}

void MyApp::update() {}

void MyApp::initializeGame() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      this->gameState[i][j].state = CellState::empty;
    }
  }
  gameRunning = 1;
  currentPlayer = 1;
}

void MyApp::updateResult() {
  for (int i = 0; i < 3; i++) {
    if (gameState[i][0].state == gameState[i][1].state &&
        gameState[i][1].state == gameState[i][2].state &&
        gameState[i][0].state != CellState::empty) {
      gameRunning = 2; // game over;
      winner = gameState[i][0].state == CellState::Player1 ? 1 : 2;
      return;
    }
    if (gameState[0][i].state == gameState[1][i].state &&
        gameState[1][i].state == gameState[2][i].state &&
        gameState[0][i].state != CellState::empty) {
      gameRunning = 2; // game over;
      winner = gameState[i][0].state == CellState::Player1 ? 1 : 2;
      return;
    }
  }

  if (gameState[0][0].state == gameState[1][1].state &&
      gameState[1][1].state == gameState[2][2].state &&
      gameState[0][0].state != CellState::empty) {
    gameRunning = 2; // game over;
    winner = gameState[0][0].state == CellState::Player1 ? 1 : 2;
    return;
  }
  if (gameState[0][2].state == gameState[1][1].state &&
      gameState[1][1].state == gameState[2][0].state &&
      gameState[1][1].state != CellState::empty) {
    gameRunning = 2; // game over;
    winner = gameState[0][0].state == CellState::Player1 ? 1 : 2;
    return;
  }
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (gameState[i][j].state == CellState::empty) {
        return;
      }
    }
  }
  gameRunning = 2; // game over draw;
  winner = 0;
}

void MyApp::key_pressed(uwe::Scancode scancode, bool repeat) {

  switch (scancode) {
  case uwe::Scancode::SPACE: {
    initializeGame();
    break;
    ;
  }
  }
}

void MyApp::mouse_pressed(int x, int y, uwe::Button button) {
  if (gameRunning != 1) {
    return;
  }

  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      int cellX = boardX + col * (cellSize + cellPadding);
      int cellY = boardY + row * (cellSize + cellPadding);
      if (gameState[row][col].state == CellState::empty) {
        if (x > cellX && x < cellX + cellSize && y > cellY &&
            y < cellY + cellSize) {
          if (currentPlayer == 1) {
            gameState[row][col].state = CellState::Player1;
            currentPlayer = 2;
          } else {
            gameState[row][col].state = CellState::Player2;
            currentPlayer = 1;
          }
          updateResult();
          return;
        }
      }
    }
  }
}

void MyApp::mouse_released(int x, int y, uwe::Button button) {}

void MyApp::mouse_moved(int x, int y) {}

void MyApp::drawTicTacToeBoard() {

  // Calculate the starting position of the board
  boardX =
      (400 - boardWidth) / 2; // Adjust the values according to your screen size
  boardY = (400 - boardHeight) /
           2; // Adjust the values according to your screen size

  // Draw the individual cells
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      int cellX = boardX + col * (cellSize + cellPadding);
      int cellY = boardY + row * (cellSize + cellPadding);
      if (gameState[row][col].state == CellState::empty) {
        // draw_rect(cellX,cellY,cellSize,cellSize);
      } else if (gameState[row][col].state == CellState::Player1) {
        set_draw_color(uwe::Colour::blue());
        draw_rect_fill(cellX, cellY, cellSize, cellSize);
      } else {
        set_draw_color(uwe::Colour::red());
        draw_rect_fill(cellX, cellY, cellSize, cellSize);
      }
    }
  }
  set_draw_color(uwe::Colour::black());
  // Draw the board outline using lines
  draw_line(boardX, boardY, boardX + boardWidth, boardY);  // Top line
  draw_line(boardX, boardY, boardX, boardY + boardHeight); // Left line
  draw_line(boardX, boardY + boardHeight, boardX + boardWidth,
            boardY + boardHeight); // Bottom line
  draw_line(boardX + boardWidth, boardY, boardX + boardWidth,
            boardY + boardHeight); // Right line

  // Draw vertical lines
  for (int i = 1; i < 3; ++i) {
    int lineX = boardX + i * (cellSize + cellPadding);
    draw_line(lineX, boardY, lineX, boardY + boardHeight);
  }
  // Draw horizontal lines
  for (int i = 1; i < 3; ++i) {
    int lineY = boardY + i * (cellSize + cellPadding);
    draw_line(boardX, lineY, boardX + boardWidth, lineY);
  }
  std::string msg;
  if (gameRunning == 1) {
    if (currentPlayer == 1) {
      msg = "Player 1's Move";
    } else if (currentPlayer == 2) {
      msg = "Player 2's Move";
    }
    draw_font(font15_, msg, 150, 10);
  } else if (gameRunning == 0) {
    msg = "Press SPACE to Start";
    draw_font(font15_, msg, 130, 10);
  } else if (gameRunning == 2) {
    if (winner == 1) {
      msg = "Player 1 Wins!!!";
      draw_font(font15_, msg, 130, 10);
    } else if (winner == 2) {
      msg = "Player 2 Wins!!!";
      draw_font(font15_, msg, 130, 10);
    } else {
      msg = "Match Drawed";
      draw_font(font15_, msg, 140, 10);
    }
    msg = "Press SPACE to Re-Start";
    draw_font(font15_, msg, 110, 25);
  }
}

void MyApp::draw() {
  clear(uwe::Colour::white());
  set_draw_color(uwe::Colour::black());
  drawTicTacToeBoard();
}

int main(int argc, char *argv[]) {
  uwe::App *app = new MyApp{width, height, "Tic-Tac-Toe"};

  app->run();

  return 0;
}