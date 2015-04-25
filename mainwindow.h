#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

  void defaultShapesColors();
 protected:
  // При нажатии кнопки на клавиатуре
  void keyPressEvent(QKeyEvent* event);

  // Отрисовка поля игры
  void paintEvent(QPaintEvent*);

 private slots:
  void on_about_triggered();

  void on_saveGame_triggered();

  void on_loadGame_triggered();

  void on_gameSettings_triggered();

  // Один шаг игры (движение фигуры вниз)
  // + проверка на столкновения
  // Уничтожение заполненных строк
  // Появление новой фигуры
  void gameStep();

 private:
  Ui::MainWindow* ui;

  Settings settings; // Настройки
  Game* game; // Состояние игры
  QMap<char, QBrush> brush; // Цвета фигур
  QTimer* timer; // Таймер для движения фигур
};

#endif // MAINWINDOW_H
