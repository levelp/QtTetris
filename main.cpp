#include "mainwindow.h"
#include "tests.h"
#include <QApplication>
#include <QTextCodec>
#include <QTest>

int main(int argc, char* argv[]) {
  // Запуск тестов: QtTetris.exe test
  if(argc >= 2 && !strcmp(argv[1], "test")) {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    return QTest::qExec(new Tests());
  } else {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
  }
}
