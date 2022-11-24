#ifndef TOWER_H
#define TOWER_H

#include "square.h"
#include "game.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QObject>


class Tower : public Square {
public:
    Tower(QWidget *parent = nullptr);
    Tower(QWidget *parent, int& x, int& y);
    QPointF towerCenter();
private:
    int range = 5;
    QGraphicsEllipseItem* attack_area;
    QPointF attack_point;
};

#endif // TOWER_H
