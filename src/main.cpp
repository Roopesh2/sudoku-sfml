#include "./sudoku.cpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>

#define CELL_SIZE 80
#define offset 240
sf::Texture texture("assests/bg.png");
sf::Font font("assests/type.ttf");
sf::Text text(font);
std::vector<Cell> cells;
sf::Time elapsed1;
sf::Image image("assests/sudoku.png");
sf::Texture drawing("assests/drawing.png");
sf::Texture notDrawing("assests/not-drawing.png");
sf::Sprite icon(notDrawing);
void sudokuGenerator(int k) {
  Sudoku *suk = new Sudoku();
  suk->fillDiagonal(cells);
  suk->solveSudoku(cells, 0, 0);
  suk->removeKDigits(cells, k);
  delete suk;
}

bool checkForVictory() {
  Sudoku *suk = new Sudoku();
  bool win = suk->isValid(cells);
  delete suk;
  return win;
}
void pencilAppend(std::string numToAdd, int index) {
  cells[index].isPencil = true;
  int found = cells[index].pencilNums.find(numToAdd);
  if (found == -1) {
    cells[index].pencilNums.append(numToAdd);
    int len = cells[index].pencilNums.size();
    if (len == 5 || len == 11) {
      cells[index].pencilNums.append("\n");
    } else {
      cells[index].pencilNums.append(" ");
    }
  } else {

    cells[index].pencilNums = cells[index].pencilNums.erase(found, 2);
  }
}
void clearSelected(int index) { cells[index].isSelected = false; }
void fillCell() {
  for (int a = 0; a < 9; a++) {
    for (int b = 0; b < 9; b++) {
      cells.push_back(Cell(b, a));
    }
  }
  sudokuGenerator(2);
}
int getMousPos(sf::RenderWindow &window) {
  int mouse_x = sf::Mouse::getPosition(window).x / CELL_SIZE;
  int mouse_y = sf::Mouse::getPosition(window).y / CELL_SIZE;
  mouse_y = mouse_y - 3;
  if (mouse_y < 0) {
    return 0;
  }
  if (cells[mouse_x + mouse_y * 9].canBeChanged)
    cells[mouse_x + mouse_y * 9].isSelected = true;
  return (mouse_x + mouse_y * 9);
}

void startScreen(bool startedPlaying, sf::RenderWindow &window) {
  if (!startedPlaying) {
    sf::RectangleShape play({90, 60});
    play.setPosition({300, 300});
    play.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setString("PLAY");
    window.draw(play);
    text.setPosition({300, 300});
    window.draw(text);
  }
}
void drawRectangles(sf::RenderWindow &window, bool isWin, bool isStarted) {
  if (isStarted) {
    if (isWin) {
      window.clear();
      int timeTaken = (elapsed1.asSeconds());
      text.setFillColor(sf::Color::White);
      text.setPosition({200, 300});
      text.setString("YOU WIN!\n" + std::to_string(timeTaken) + " seconds");
      window.draw(text);
      return;
    }
    sf::RectangleShape rectangle({CELL_SIZE, CELL_SIZE});
    sf::RectangleShape line({5, 1100});
    line.setFillColor(sf::Color::Black);
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (cells[i + j * 9].isSelected) {
          rectangle.setFillColor(sf::Color(99, 99, 99));
        } else {
          rectangle.setFillColor(sf::Color::White);
        }
        rectangle.setOutlineThickness(1);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition(
            {float(CELL_SIZE * i), float(CELL_SIZE * j) + offset});
        rectangle.setTexture(&texture);
        window.draw(rectangle);
        text.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j) + offset});
        if (cells[i + j * 9].canBeChanged && !cells[i + j * 9].isPencil &&
            cells[i + j * 9].number != 0) {
          text.setCharacterSize(64);
          text.setFillColor(sf::Color::Black);
          std::string String = std::to_string(cells[i + j * 9].number);
          String = " " + String;
          text.setString(String);
          window.draw(text);
        } else if (cells[i + j * 9].isPencil) {
          text.setCharacterSize(21);
          text.setFillColor(sf::Color::Black);
          text.setString(cells[i + j * 9].pencilNums);
          window.draw(text);
        } else if (cells[i + j * 9].number != 0) {
          std::string String = std::to_string(cells[i + j * 9].number);
          String = " " + String;
          text.setString(String);
          text.setCharacterSize(64);
          text.setFillColor(sf::Color(99, 62, 21));
          window.draw(text);
        }
        if ((i % 3 == 0 && i != 0)) {
          line.setSize({5, 1100});
          line.setPosition({float(CELL_SIZE * i - 5), 0});
          window.draw(line);
        } else if ((j % 3 == 0 && j != 0)) {
          line.setSize({1100, 5});
          line.setPosition({0, float(CELL_SIZE * j) + offset});
          window.draw(line);
        }
      }
    }
  }
}
void drawTime(sf::RenderWindow &window, sf::Time time, bool isStart) {
  if (isStart) {
    icon.scale({1, 1});
    icon.setPosition({30, 100});
    window.draw(icon);
    text.setPosition({348, 0});
    text.setFillColor(sf::Color::White);
    std::string str = std::to_string(time.asSeconds());
    std::string newStr = "";
    for (int i = 0; i < str.length(); i++) {
      if (str[i] == '.')
        break;
      newStr.append(1, str[i]);
    }
    text.setString(newStr);
    window.draw(text);
  }
}
int main() {
  bool isPencil = false;
  bool start = false;
  srand(time(0));
  fillCell();
  sf::Clock clock;
  clock.stop();
  bool isWin = false;
  sf::RenderWindow window =
      sf::RenderWindow(sf::VideoMode({9 * 80, 9 * 80 + offset}), "Sudoku");
  window.setIcon(image.getSize(), image.getPixelsPtr());
  window.setFramerateLimit(144);
  int index = -1;
  while (window.isOpen()) {
    elapsed1 = clock.getElapsedTime();
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (checkForVictory()) {
        isWin = true;
        elapsed1 = clock.getElapsedTime();
        clock.stop();
      }
      if (event->is<sf::Event::Resized>()) {
        window.setSize({9 * 80, 9 * 80 + offset});
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !start) {
        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;
        if ((x < 380 && x > 300) && (y < 380 && y > 300)) {
          start = true;
          clock.reset();
          clock.start();
        }
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (index != -1)
          clearSelected(index);
        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;
        if ((x >= 30 && y >= 100 && y <= 200 && x <= 120)) {
          if (isPencil) {
            icon.setTexture(notDrawing);
            isPencil = false;
          } else {
            icon.setTexture(drawing);
            isPencil = true;
          }
        }
        index = getMousPos(window);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("1", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 1;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";
            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("2", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 2;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("3", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 3;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("4", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 4;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num5)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("5", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 5;
            clearSelected(index);
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num6)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("6", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 6;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num7)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("7", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 7;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num8)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("8", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 8;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num9)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("9", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 9;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num0)) {
        if (index != -1) {
          cells[index].number = 0;
          cells[index].isPencil = false;
          cells[index].pencilNums = "";
          clearSelected(index);
          index = -1;
        }
      }
    }
    window.clear();
    startScreen(start, window);
    drawTime(window, elapsed1, start);
    drawRectangles(window, isWin, start);
    window.display();
  }
}
