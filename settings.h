#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

// Настройки игры
class Settings : public QSettings {
  static const QString GlassHeight;
  static const QString GlassWidth;
  static const QString CellSize;
  static const QString CellBorder;
 public:
  Settings();

  // Ширина стакана
  int glassWidth() {
    return this->value(GlassWidth, 10).toInt();
  }

  void setGlassWidth(int v) {
    this->setValue(GlassWidth, v);
  }

  // Высота стакана
  int glassHeight() {
    return this->value(GlassHeight, 20).toInt();
  }

  void setGlassHeight(int v) {
    this->setValue(GlassHeight, v);
  }

  // Размер клетки в пикселах (без бордюра)
  int cellSize() {
    return this->value(CellSize, 20).toInt();
  }

  void setCellSize(int v) {
    this->setValue(CellSize, v);
  }

  int cellBorder() {
    return this->value(CellBorder, 1).toInt();
  }

  void setCellBorder(int v) {
    this->setValue(CellBorder, v);
  }

  int cellSizeX() {
    return 2 * cellBorder() + cellSize();
  }
};

#endif // SETTINGS_H
