/**
 * @file enemy.cpp
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "enemy.h"
#include "game.h"
/**
 * @brief Construct a new Enemy:: Enemy object,  the necessary members are initialized.
 * 
 * @param type 
 * @param path 
 * @param matrixPath 
 * @param health 
 * @param damage 
 * @param speed 
 * @param parent 
 */
Enemy::Enemy(EnemyType type, QList<QPointF> path,QList<QPoint> matrixPath, int health , int damage , int speed , QGraphicsItem * parent):
     QGraphicsPixmapItem(parent), health_(health), damage_(damage), speed_(speed),path_(path),matrixPath_(matrixPath), type_(type)
{


    setPos(path_[0]);
    point_index_ = 0;
    dest_ = path_[0];
    timer_ = new QTimer(this);

}
/**
 * @brief Is called when the enemy takes damage from towers.
 * Either decreases the enemy's health or calls die().
 * 
 * @param damage The damage of the projectile, fired by a tower.
 */
void Enemy::takeDamage(int damage)
{
    qobject_cast<Game*>(scene())->playHitsound();
    health_-=damage;
    if(health_<=0)
    {
        die();
    }
}
/**
 * @brief Is called when the enemy dies.
 * Emits a signal to notify that the enemy has died and calls deleteLater().
 * 
 */
void Enemy::die()
{
    emit enemyDies(pointValue_);
    deleteLater();

}
/**
 * @brief Sets the speed of the enemy.
 * 
 * @param speed The new speed of the enemy.
 */
void Enemy::setSpeed(int speed)
{
    speed_=speed;
}
/**
 * @brief Starts the timer that calls move() after the specified interval.
 * 
 */
void Enemy::startMove()
{

    connect(timer_,SIGNAL(timeout()),this,SLOT(move()));
    timer_->start(90-speed_);
}
/**
 * @brief Sets the path of the enemy.
 * 
 * @param matrixPath The grid representation of the path.
 * @param path The QPointF(Scene pixels) representation of the path.
 */
void Enemy::setPath(QList<QPoint> matrixPath,QList<QPointF> path)
{
    path_=path;
    matrixPath_=matrixPath;
    point_index_=0;
}
/**
 * @brief Returns the grid location of the enemy.
 * 
 * @return QPoint 
 */
QPoint Enemy::getMatrixLocation() const
{
    if (point_index_ > 0) {
        return matrixPath_[point_index_-1];
    } else {
        return matrixPath_[point_index_];
    }
}
/**
 * @brief Returns the timer of the enemy. 
 * 
 * @return QTimer* 
 */
QTimer *Enemy::getTimer()
{
    return timer_;
}
/**
 * @brief Returns the next location of the enemy.
 * 
 * @return QPoint 
 */
QPoint Enemy::getNextLocation() const
{
    return matrixPath_[point_index_];
}
/**
 * @brief Returns the type of the enemy.
 * 
 * @return EnemyType 
 */
EnemyType Enemy::getType() const {
    return type_;
}
/**
 * @brief Moves the enemy along on the path by STEP_SIZE. 
 * Emits a signal dealsDamage(int) when reaches the final destination which is handles in game.cpp.
 * 
 */
void Enemy::move()
{
    QLineF ln(pos(),dest_);

    if (ln.length() == 0)
    {

        point_index_++;
        // last point reached
        if (point_index_ >= path_.size()){

           emit dealsDamage(damage_);


            return;
        }

        QPoint nextPos = matrixPath_[point_index_];

        if(qobject_cast<Game*>(scene())->isComment(nextPos.x(),nextPos.y()))
        {
            timer_->stop();
            qobject_cast<Game*>(scene())->breakComment(nextPos.x(),nextPos.y());
            return;
        }
        // last point not reached, get new destination
        dest_ = path_[point_index_];
        // qInfo() << nextPos;
        ln = QLineF (pos(),dest_);
    }

    // move enemy forward
    int STEP_SIZE = 2;
    double angle = -1 * ln.angle();
    double dy = STEP_SIZE * qSin(qDegreesToRadians(angle));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(angle));

    setPos(x()+dx, y()+dy);
}

