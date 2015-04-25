#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent> // Нажатия кнопок на клавиатуре
#include <QDebug> // Вывод в отладочную консоль
#include <QTimer>
#include <QPainter> // Рисование на форме
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "game.h"
#include "gamesettings.h"

void MainWindow::defaultShapesColors() {
  brush.insert('_', QBrush(Qt::NoBrush)); // Для пустой клетки
  brush.insert('I', QBrush(Qt::red));
  brush.insert('J', QBrush(Qt::blue));
  brush.insert('L', QBrush(Qt::magenta));
  brush.insert('O', QBrush(Qt::gray));
  brush.insert('S', QBrush(Qt::green));
  brush.insert('T', QBrush(Qt::cyan));
  brush.insert('Z', QBrush(QColor::fromRgb(153, 255, 0))); // lime
}

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  game = new Game(settings.glassWidth(), settings.glassHeight());

  defaultShapesColors();

  timer = new QTimer(this);
  // Соединяем сигнал со слотом
  connect(timer,
          SIGNAL(timeout()),
          this,
          SLOT(gameStep()));
  game->timerInterval = 200;
  timer->start(game->timerInterval);

  QSize glassSize = ui->glass->maximumSize();
  int borders = 2 * ui->glass->lineWidth() + 4;
  glassSize.setWidth(game->width() * settings.cellSizeX() + borders);
  glassSize.setHeight(game->height() * settings.cellSizeX() + borders);
  ui->glass->setMinimumSize(glassSize);
  ui->glass->setMaximumSize(glassSize);

  game->start();
}

MainWindow::~MainWindow() {
  delete game;
  delete ui;
}

void MainWindow::on_about_triggered() {
  About about;
  // Модальное окно - пока не закроешь,
  // остальное приложение блокируется
  about.exec();
}

void MainWindow::on_saveGame_triggered() {
  QString filename = QFileDialog::getSaveFileName(
                       this, "Сохранить игру", "",
                       "Тетрис (*.tetris)");

  if(filename.isEmpty())
    return;

  try {
    game->save(filename);
  } catch(QString error) {
    // critical(QWidget * parent, const QString & title,
    // const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton)
    QMessageBox::critical(this, "Ошибка при сохранении файла", error);
  }
}

void MainWindow::on_loadGame_triggered() {
  QString filename = QFileDialog::getOpenFileName(
                       this, "Загрузить игру", "",
                       "Тетрис (*.tetris)");

  try {
    game->load(filename);
  } catch(QString error) {
    QMessageBox::critical(this, "Ошибка при чтении файла", error);
  }

  //updateWindow();
}

void MainWindow::on_gameSettings_triggered() {
  GameSettings form(this, settings);
  form.exec();
}

// При нажатии на кнопку на клавиатуре
void MainWindow::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Left: // Двигаем фигуру влево
      qDebug() << "Left";
      game->move(-1, 0);
      update();
      break;

    case Qt::Key_Right: // Двигаем фигуру вправо
      qDebug() << "Right";
      game->move(+1, 0);
      update();
      break;

    case Qt::Key_Down: // Приземляем фигуру
      qDebug() << "Down - landed figure";
      game->move(0, +1);
      update();
      break;

    case Qt::Key_Up: // Поворачиваем фигуру
      qDebug() << "Up - rotate";
      game->rotate();
      update();
      break;

    case Qt::Key_Space: // Пробел - пауза
      if(game->paused) {
        qDebug() << "Resume the game";
        timer->start();
        game->paused = false;
      } else {
        qDebug() << "Game pause";
        timer->stop();
        game->paused = true;
      }

      break;
  }

  // Вызываем стандартный обработчик нажатий
  QMainWindow::keyPressEvent(event);
}

void MainWindow::gameStep() {
  // Один шаг игры (движение фигуры вниз)
  // + проверка на столкновения
  // Уничтожение заполненных строк
  // Появление новой фигуры

  // Шаг игры
  game->step();

  // Обновление статистики игры: счёта, количества строк
  ui->Score->setText(QString("%1").arg(game->score));
  ui->Lines->setText(QString("%1").arg(game->lines));
  ui->Level->setText(QString("%1").arg(game->level));
  ui->TimerInterval->setText(QString("%1").arg(game->timerInterval));
  // Обновляем период работы таймера
  timer->setInterval(game->timerInterval);

  // Перерисовка для движения фигур
  update();
}

// Отрисовка поля игры
void MainWindow::paintEvent(QPaintEvent*) {
  QPainter p(this);

  const int cellBorder = settings.cellBorder();
  const int cellSize = settings.cellSize();
  const int cellSizeX = settings.cellSizeX();

  QRect g1 = ui->centralWidget->geometry();
  QRect g2 = ui->glass->geometry();
  const int xLeft = g1.left() + g2.left() + ui->glass->lineWidth() + 1;
  const int yTop = g1.top() + g2.top() + ui->glass->lineWidth() + 1;

  // Рисуем стакан
  p.drawRect(QRect(xLeft, yTop,
                   game->width() * cellSizeX,
                   game->height() * cellSizeX));

  // Рисуем клетки
  // Направим ось x вправо
  // Ось y - вниз
  for(int i = 0; i < game->height(); ++i)
    for(int j = 0; j < game->width(); ++j) {
      p.setBrush(brush[game->glass[i][j]]);
      p.drawRoundedRect(
        QRect(xLeft + j * cellSizeX + cellBorder,
              yTop + i * cellSizeX + cellBorder,
              cellSize, cellSize), 2, 2);
    }
}


