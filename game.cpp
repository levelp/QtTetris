#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QDebug> // Для отладки
#include <cstring>
#include <cassert>
#include "game.h"

using namespace std;

void Game::memAlloc() {
  glass = new char* [h];

  for(int i = 0; i < h; ++i) {
    glass[i] = new char[w + 1];
    memset(glass[i], '_', sizeof(char) * w); // Строка из '_'
    glass[i][w] = 0; // заканчивается 0
  }
}

void Game::memFree() {
  for(int i = 0; i < h; ++i)
    delete[] glass[i];

  delete[] glass;
  glass = NULL;
}

Game::Game(int width, int height) : w(width), h(height),
  paused(false), level(1) {
  memAlloc();
}

Game::~Game() {
  memFree();
}

// Сохранение игры в файл
void Game::save(QString fileName) {
  QFile f(fileName); // QFile позволяет работать с файлами
  f.open(QIODevice::WriteOnly | QIODevice::Text);
  // Сохраняем данные
  QTextStream out(&f);
  // Ширина и высота стакана
  out << w << " " << h << endl;

  // Содержимое стакана
  for(int i = 0; i < h; ++i) {
    for(int j = 0; j < w; ++j)
      out << glass[i][j];

    out << endl;
  }

  f.close();
}

// Загрузка игры из файла
void Game::load(QString fileName) {
  QFile f(fileName); // Создаем объект - файл
  f.open(QIODevice::ReadOnly | QIODevice::Text); // Открываем его для чтения
  // Читаем данные
  QTextStream in(&f);

  memFree();

  // Считываем размер карты
  in >> w >> h;
  in.readLine();

  memAlloc();

  for(int i = 0; i < h; ++i) {
    in >> glass[i];
    in.readLine(); // Строчка должна была закончиться
  }

  f.close();
}

void Game::drawCell(int i, int j, char c) {
  // Если клетка за границами стакана
  // т.е. координаты меньше 0
  // Или больше или равны размеров стакана
  // То не рисуем эту клетку фигуры
  if(i < 0 || j < 0 || i >= h || j >= w)
    return;

  // Если клетка фигуры внутри стакана, перекрашиваем
  // в цвет клетки "квадратик" стакана
  glass[i][j] = c;
}

void Game::draw(const Shape& shape) {
  // Рисуем фигуру её именем внутри стакана
  for(int i = 0; i < SIZE; ++i)
    for(int j = 0; j < SIZE; ++j)
      if (shape.image[i][j] == '#')
        drawCell(i + shape.y, j + shape.x, shape.name);
}

void Game::erase(const Shape& shape) {
  // Рисуем фигуру её именем внутри стакана
  for(int i = 0; i < SIZE; ++i)
    for(int j = 0; j < SIZE; ++j)
      if (shape.image[i][j] == '#')
        drawCell(i + shape.y, j + shape.x, '_');
}

// Помещается ли фигура в это место стакана?
// true - помещается
// false - не помещается
bool Game::fit(const Shape& shape) {
  for(int i = 0; i < SIZE; ++i)
    for(int j = 0; j < SIZE; ++j)
      if (shape.image[i][j] == '#') {
        int y = i + shape.y;
        int x = j + shape.x;

        // Если
        if(y < 0) continue; // Выплывает сверху - и это нормально

        // Выходит за правую или левую границу стакана
        if(x < 0 || x >= w) return false;

        // Ниже "дна" стакана
        if(y >= h) return false;

        // Накладывается на занятую клетку
        if(glass[y][x] != '_')
          return false; // Не поместилась
      }

  return true; // Поместилась!
}

// Пробуем подвинуть фигуру на dx, dy
// Результат - удалось/не удалось
bool Game::move(int dx, int dy) {
  erase(curShape);
  // Делаем копию фигуры
  Shape s = curShape;
  // Двигаем копию
  s.x += dx;
  s.y += dy;
  // Если поместилась
  bool res = fit(s);

  if(res)
    curShape = s;

  draw(curShape);
  return res;
}

// Пробуем повернуть фигуру
void Game::rotate() {
  erase(curShape);
  // Делаем копию фигуры
  Shape s = curShape;
  // Поворачиваем копию
  s.rotate();

  // Если повёрнутая фигура встаёт => запоминаем
  if(fit(s))
    curShape = s;

  draw(curShape);
}

void Game::genNextShape() {
  // Выбираем случайную фигуру
  curShape = shapes[rand() % SHAPES_NUM];
  // Помещаем её посередине сверху
  curShape.y = -SIZE;
  curShape.x = w / 2 - SIZE / 2;
  // Поворачиваем случайным образом
  int rotations = rand() % 4;

  for(int i = 0; i < rotations; ++i)
    curShape.rotate();
}

// Начало игры
void Game::start() {
  score = 0;
  lines = 0;
  // Генерируем новую фигуру
  genNextShape();
}

// Один шаг игры
void Game::step() {
  if(!move(0, +1)) {
    genNextShape();
    checkFullLines();
  }
}

// Проверка, есть ли полные строки
void Game::removeFullLines(int from, int to) {
  assert(to <= from); // Строки снизу вверх
  int block = from - to + 1;
  qDebug() << "block:" << block;

  lines += block;
  score += block * level;

  // Сдивигаем содержимое стакана на block строк вниз
  for(int r = to - 1; r >= 0; r--)
    for(int y = 0; y < w; ++y)
      glass[r + block][y] = glass[r][y];

  // Очищаем верхние строки стакана
  for(int r = block - 1; r >= 0; r--)
    for(int y = 0; y < w; ++y)
      glass[r][y] = '_';
}

void Game::checkFullLines() {
  int fromRow = -1, toRow = -1; // от строки до строки

  // Бежим по стакану снизу вверх
  for(int r = h - 1; r >= 0; --r) {
    // Считаем количество заполненных клеток
    int cnt = 0;

    for(int x = 0; x < w; ++x)
      if(glass[r][x] != '_')
        cnt++;

    // Если линия заполненна
    if(cnt == w) {
      // Первая заполненная
      if(fromRow == -1)
        fromRow = r;

      toRow = r; // Последняя заполненная линия
    } else { // Если линия не заполнена..
      // ..и уже был блок => то его надо уничтожить :)
      // добавить игроку очки и т.д.
      if(fromRow != -1 && toRow != -1) {
        removeFullLines(fromRow, toRow);
        return;
      }
    }
  }
}
