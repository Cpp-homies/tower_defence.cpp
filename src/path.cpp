#include "path.h"
#include <QPixmap>

Path::Path(int x, int y, PathType , PathConfig config, int rotation, QWidget* parent)
        : Square(x, y, parent) {
    // Set up rotation transform
    QTransform transform;
    transform.rotate(rotation);

    switch (config) {
    case Straight:
        setPixmap(QPixmap(":/images/Path_tile_vertical.png").transformed(transform));
        break;
    case Turn:
        setPixmap(QPixmap(":/images/Path_tile_top_right.png").transformed(transform));
        break;
    case TSplit:
        setPixmap(QPixmap(":/images/Path_tile_T-shape.png").transformed(transform));
        break;
    case XSplit:
        setPixmap(QPixmap(":/images/Path_tile_x-shape.png").transformed(transform));
        break;
    }
};
