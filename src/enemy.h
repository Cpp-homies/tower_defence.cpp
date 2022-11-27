#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include <QGraphicsPixmapItem>

//subtypes of enemies, doubles as a stats multiplier?
enum class EnemyType
{
    CompilerError=1,
    MemoryError=2,
    RuntimeError=3
};

enum CompilerErrorType
{
    SyntaxError=1,
    Exception=2
};
enum MemoryErrorType
{
    InvalidRead=3,
    InvalidWrite=4,
    XBytesAreLost=5,
    MismatchedDeleteFree=6
};
enum RuntimeErrorType
{
    MemoryStackMinion=1,
    StackOverflow=2
};
class Enemy: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(EnemyType type, QList<QPointF> path, Game& game, int health = 0, int damage = 0, int speed = 0, QGraphicsItem * parent=0);

    virtual void attack(){}
    virtual void die();
    virtual void takeDamage(int damage);
    void setSpeed(int speed);
    void startMove();

public slots:

    void move();


protected:

    int health_;
    int damage_;
    int speed_; // 0 to 70
    int pointValue_;
    Game& game_;
    QList<QPointF> path_;
    QPointF dest_;
    int point_index_;
    EnemyType type_;
};

#endif // ENEMY_H
