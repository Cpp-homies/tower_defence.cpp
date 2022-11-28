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
    game_.enemyDies();
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
            deleteLater();

            if(game_.isLost())
            {
                emit game_.gameLost();
            }

            return;
        }
        // last point not reached, get new destination
        dest_ = path_[point_index_];
        ln = QLineF (pos(),dest_);



    }

    // move enemy forward
    int STEP_SIZE = 2;
    double angle = -1 * ln.angle();
    double dy = STEP_SIZE * qSin(qDegreesToRadians(angle));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(angle));

    setPos(x()+dx, y()+dy);
}

