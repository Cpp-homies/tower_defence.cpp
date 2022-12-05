#ifndef COMMENT_H
#define COMMENT_H

#include "path.h"
#include <QPointF>

class Comment : public Path {
public:
    Comment(int x, int y, int duration, Path* old, QWidget* parent);
    void changeHealth(int value);
    QPointF towerCenter();
private:
    int duration_;
    QGraphicsPixmapItem * towerImg;
    Path* old_;
};

#endif // COMMENT_H
