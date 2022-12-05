#include "path.h"
#include <QPixmap>

Path::Path(int x, int y, PathType type, int rotation, Path* old, QWidget* parent)
        : Square(x, y, parent), old_(old) {
    isComment_ = (type == CommentType);
    // Set up rotation transform
    QTransform transform;
    transform.rotate(rotation);

    switch (type) {
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
    case Start:
    case End:
        setPixmap(QPixmap(":/images/Path_startend.png").transformed(transform));
        break;
    case CommentType:
        if (old != nullptr){
            setPixmap(old_->pixmap());
        }
        break;
    }
};
