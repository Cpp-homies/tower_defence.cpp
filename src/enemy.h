#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include <QGraphicsPixmapItem>

class Enemy: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(int health, int damage, int speed,QList<QPointF> path, Game& game, QGraphicsItem * parent=0);

    virtual void attack(){}
    virtual void die();
    void takeDamage(int damage);

public slots:

    void move();


private:

    int health_;
    int damage_;
    int speed_;

    Game& game_;

    QList<QPointF> path_;
    QPointF dest_;
    int point_index_;
};

#endif // ENEMY_H
