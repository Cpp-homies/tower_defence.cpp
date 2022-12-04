#include "projectile.h"
#include "enemy.h"
#include <QPixmap>
#include <QTimer>
#include <qmath.h>


Projectile::Projectile(int damage, QString imgPath,
                       int pierce, QGraphicsItem *parent)
    : QObject(),QGraphicsPixmapItem(parent), damage_(damage), pierce_(pierce)
{
    setPixmap(QPixmap(imgPath));
    setTransformOriginPoint(pixmap().width()/2,pixmap().height()/2);
    QTimer * move_timer = new QTimer(this);
    connect(move_timer,SIGNAL(timeout()),this,SLOT(move()));
    move_timer->start(10);

    maxRange_ = 200;
    distanceTravelled_ = 0;
    pierceCount_ = 0;
}

void Projectile::move(){
    int STEP_SIZE = 6;
    double theta = rotation(); // degrees

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    setPos(x()+dx, y()+dy);
    for (auto item : collidingItems())
    {
        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if(enemy)
        {
            if (!enemiesHit_.contains(enemy)){
                enemy->takeDamage(damage_);
                enemiesHit_.append(enemy);
                pierceCount_++;
            }
            if (pierceCount_ >= pierce_){
                deleteLater();
            }
            // update distance travel here as well
            setDistanceTravelled(distanceTravelled_+STEP_SIZE);
            if(distanceTravelled_>maxRange_) deleteLater();
            return;
        }

    }


    setDistanceTravelled(distanceTravelled_+STEP_SIZE);
    if(distanceTravelled_>maxRange_) deleteLater();
}

double Projectile::getMaxRange(){
    return maxRange_;
}

double Projectile::getDistanceTravelled(){
    return distanceTravelled_;
}

void Projectile::setMaxRange(double range){
    maxRange_ = range;
}

void Projectile::setDistanceTravelled(double distance){
    distanceTravelled_ = distance;
}
