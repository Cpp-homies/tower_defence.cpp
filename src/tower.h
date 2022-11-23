#ifndef TOWER_H
#define TOWER_H

#include "square.h"
#include "game.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>


class Tower : public Square {
public:
    Tower(QWidget *parent = nullptr);
private:
    int range = 20;
    QGraphicsEllipseItem* attack_area;
};

#endif // TOWER_H
