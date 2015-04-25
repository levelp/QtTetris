#include "settings.h"

const QString Settings::GlassHeight = "glass/height";
const QString Settings::GlassWidth = "glass/width";
const QString Settings::CellSize = "glass/cellSize";
const QString Settings::CellBorder = "glass/cellBorder";

Settings::Settings() : QSettings("tetris.ini", QSettings::IniFormat) {
}
