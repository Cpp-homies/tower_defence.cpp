#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include <QGraphicsPixmapItem>

enum EnemyName
{
    SyntaxError=1,
    Exception=2,
    InvalidRead=3,
    InvalidWrite=4,
    XBytesAreLost=5,
    MismatchedDeleteFree=6,
    StackOverflow=7
};

class Enemy: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(QList<QPointF> path, Game& game, int health = 0, int damage = 0, int speed = 0, QGraphicsItem * parent=0);

    virtual void attack(){}
    virtual void die() = 0;
    void takeDamage(int damage);
    void setSpeed(int speed);
    void startMove();

public slots:

    void move();


protected:

    int health_;
    int damage_;
    int speed_; // 0 to 70
    int pointValue_;
    EnemyName name_;
    Game& game_;
    QList<QPointF> path_;
    QPointF dest_;
    int point_index_;
};

#endif // ENEMY_H
