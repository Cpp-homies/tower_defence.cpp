/**
 * @file path.cpp
 * @authors rv (harvey.lim@aalto.fi), Saku Kovanen (saku.kovanen@aalto.fi)
 * @brief File containing the path tile methods.
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "path.h"
#include <QPixmap>

/**
 * @brief The constructor class for the Path.
 * @param x The x-coordinate of the path.
 * @param y The y-coordinate of the path.
 * @param type The type of path.
 * @param rotation The rotational orientation of the path, in degrees.
 * @param old If the path contains a comment tower, this will hold the path tile the comment tower replaced when it was built.
 * @param parent The parent widget of the path.
 */
Path::Path(int x, int y, PathType type, int rotation, Path* old, QWidget* parent)
        : Square(x, y, parent), old_(old), type_(type), rotation_(rotation) {
    isComment_ = (type == CommentType);
    // Set up rotation transform
    QTransform transform;
    transform.rotate(rotation);
    // Set the pixmap with the correct image according to the type given.
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
    case CommentType: // If it is a comment type, restore the old path image in the background of the comment.
        if (old != nullptr){
            setPixmap(old_->pixmap());
        }
        break;
    }
};

/**
 * @brief Gets the type of the path.
 * @return The type of the path.
 */
PathType Path::getType() {
    return type_;
}
/**
 * @brief Gets the rotational orientation of the path.
 * @return The rotational orientation of the path, in degrees.
 */
int Path::getRotation() {
    return rotation_;
}
