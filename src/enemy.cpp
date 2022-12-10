#include "enemy.h"
#include "game.h"

Enemy::Enemy(EnemyType type, QList<QPointF> path,QList<QPoint> matrixPath, int health , int damage , int speed , QGraphicsItem * parent):
     QGraphicsPixmapItem(parent), health_(health), damage_(damage), speed_(speed),path_(path),matrixPath_(matrixPath), type_(type)
{


    setPos(path_[0]);
    point_index_ = 0;
    dest_ = path_[0];
    timer_ = new QTimer(this);

}

void Enemy::takeDamage(int damage)
{
    qobject_cast<Game*>(scene())->playHitsound();
    health_-=damage;
    if(health_<=0)
    {
        die();
    }
}

void Enemy::die()
{
    emit enemyDies(pointValue_);
    deleteLater();

}

void Enemy::setSpeed(int speed)
{
    speed_=speed;
}

void Enemy::startMove()
{

    connect(timer_,SIGNAL(timeout()),this,SLOT(move()));
    timer_->start(90-speed_);
}

void Enemy::setPath(QList<QPoint> matrixPath,QList<QPointF> path)
{
    path_=path;
    matrixPath_=matrixPath;
    point_index_=0;
}

QPoint Enemy::getMatrixLocation() const
{
    return matrixPath_[point_index_-1];
}

QTimer *Enemy::getTimer()
{
    return timer_;
}

QPoint Enemy::getNextLocation() const
{
    return matrixPath_[point_index_];
}

EnemyType Enemy::getType() const {
    return type_;
}

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

