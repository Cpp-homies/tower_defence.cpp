#include "enemy.h"

#include <QTimer>

Enemy::Enemy(EnemyType type, QList<QPointF> path, Game& game, int health , int damage , int speed , QGraphicsItem * parent):
     QGraphicsPixmapItem(parent), health_(health), damage_(damage), speed_(speed), game_(game),path_(path), type_(type)
{


    setPos(path_[0]);
    point_index_ = 0;
    dest_ = path_[0];


}

void Enemy::takeDamage(int damage)
{
    health_-=damage;
    if(health_<=0)
    {
        die();
    }

}

void Enemy::die()
{
    game_.changeScore(pointValue_);
    game_.changeCurrency(pointValue_);
    deleteLater();
}

void Enemy::setSpeed(int speed)
{
    speed_=speed;
}

void Enemy::startMove()
{
    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(90-speed_);
}

void Enemy::move()
{
    QLineF ln(pos(),dest_);
    if (ln.length() == 0)
    {
        point_index_++;
        // last point reached
        if (point_index_ >= path_.size()){
            game_.changeHealth(-damage_);
            if(game_.isLost())
            {
                //game is lost here
            }
            deleteLater();
            return;
        }
        // last point not reached
        dest_ = path_[point_index_];
        ln = QLineF (pos(),dest_);
        setRotation(-1 * ln.angle());


    }

    // move enemy forward at current angle
    int STEP_SIZE = 2;
    double theta = rotation(); // degrees

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    setPos(x()+dx, y()+dy);
}

