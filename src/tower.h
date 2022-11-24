#ifndef TOWER_H
#define TOWER_H

#include "square.h"
#include "game.h"
#include "projectile.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QObject>
#include "mainview.h"

extern MainView* view;

class Tower : public Square {
public:
    Tower(QWidget *parent = nullptr);
    Tower(int x, int y, QWidget *parent);
    Tower(int x, int y, QWidget *parent, int range, int damage, int attackSpeed);
    QPointF towerCenter();
    double distanceTo(QGraphicsItem * item);
    void fire(QPointF targetPos);
public slots:
    void getTarget();
private:
    int range_ = 5;
    int damage_;
    int attackInterval_;// in mili-seconds
    bool upgradeable_;
    int ugradeLevel_;
    double damageMultiplier_;
    int totalCost_;
    bool has_target_;
    QGraphicsEllipseItem* attack_area;
    QPointF target_pos;
};

// dummy class for enemy
class Enemy : public QGraphicsPixmapItem {
public:
    Enemy(QGraphicsItem * parent=nullptr) {
        setPixmap(QPixmap(":/images/syntax_error3.png"));

        // get the possition of the square at (4, 4)
//        QPointF pos = view->getGame()->getSquarePos(4, 4);
        setPos(200, 200);
    }
};

#endif // TOWER_H
