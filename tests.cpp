#include "tests.h"
#include <QDebug>
#include <QtTest/QTest>

using namespace std;

Tests::Tests(QObject* parent) :
  QObject(parent) {
}

// Инициализация всех тестов
void Tests::initTestCase() {
  qDebug() << "All tests initialization";
}

void Tests::testSimple() {
  QCOMPARE(2 * 2, 4);
  QCOMPARE(QString("Hello, ").append("World!"),
           QString("Hello, World!"));
}
