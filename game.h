#ifndef GAME_H
#define GAME_H

#include <QObject>

const int SIZE = 4;

// Одна фигура тетриса
struct Shape {
  char name; // Название фигуры
  // Изображение фигуры
  char image[SIZE][SIZE + 1]; // +1 - на символ 0
  int y, x; // Координаты

  // Поворот фигуры на 90 градусов
  // по часовой стрелке
  //  0,0  0,1  0,2  0,3
  // *1,0* 1,1  1,2  1,3
  //  2,0 #2,1# 2,2  2,3
  //  3,0  3,1  3,2  3,3
  //     \/ \/ \/ \/
  //  3,0  2,0 *1,0* 0,0
  //  3,1 #2,1# 1,1  0,1
  //  3,2  2,2  1,2  0,2
  //  3,3  2,3  1,3  0,3
  void rotate() {
    // Сохраняем текущую фигуру
    Shape old = *this;

    for(int i = 0; i < SIZE; ++i)
      for(int j = 0; j < SIZE; ++j) {
        // * (0,2) <- (1,0)
        // # (1,1) <- (2,1)
        image[j][SIZE - i - 1] = old.image[i][j];
      }
  }
};

const int SHAPES_NUM = 7;

const Shape shapes[SHAPES_NUM] = {
  {
    'Z',
    {
      "    ",
      "##  ",
      " ## ",
      "    ",
    }, 0, 0
  },
  {
    'S',
    {
      "    ",
      " ## ",
      "##  ",
      "    ",
    }, 0, 0
  },
  {
    'L',
    {
      " #  ",
      " #  ",
      " ## ",
      "    ",
    }, 0, 0
  },
  {
    'J',
    {
      "  # ",
      "  # ",
      " ## ",
      "    ",
    }, 0, 0
  },
  {
    'O',
    {
      "    ",
      " ## ",
      " ## ",
      "    ",
    }, 0, 0
  },
  {
    'I',
    {
      " #  ",
      " #  ",
      " #  ",
      " #  ",
    }, 0, 0
  },
  {
    'T',
    {
      "    ",
      "### ",
      " #  ",
      "    ",
    }, 0, 0
  }
};

// Состояние игры
class Game {
  int h, w; // Высота и ширина стакана
  void memAlloc(); // Отводим память под хранение стакана
  void memFree(); // Очищаем память при завершении игры
  // Отрисовка клетки
  void drawCell(int i, int j, char c);

  // Есть ли полные строки?
  void checkFullLines();
  // Удаление полных строк
  void removeFullLines(int fromRow, int toRow);
 public:
  // Получение высоты стакана
  int height() {
    return h;
  }
  // Получение ширины стакана
  int width() {
    return w;
  }
  bool paused; // Игра на паузе?
  int score; // Счёт игры
  int lines; // Количество уничтоженных строк
  int level; // Уровень игры
  int timerInterval; // Скорость игры

  char** glass; // Содержимое стакана
  Shape curShape; // Текущая фигура
  Game(int width, int height);
  ~Game();
  // Сохранение игры в файл
  void save(QString fileName);
  // Загрузка игры из файла
  void load(QString fileName);

  // Отрисовка фигуры
  void draw(const Shape& shape);
  // Удаление фигуры
  void erase(const Shape& shape);
  // Помещается ли фигура в это место стакана?
  bool fit(const Shape& shape);

  // Попытка передвинуть фигуру
  bool move(int dx, int dy);
  // Попытка поворота фигуры
  void rotate();

  // Начало игры
  void start();
  // Один шаг игры
  void step();

  // Генерация новой фигуры
  void genNextShape();
};

#endif // GAME_H
